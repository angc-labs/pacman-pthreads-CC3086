#include <iostream>
#include <ncurses.h>
#include "ui.h"
#include "Mapa.h"
#include "Pacman.h"
#include "Punto.h"
#include <unistd.h>
#include <vector>
#include <memory>
#include "../headers/Ghost.h"
#include <pthread.h>

std::vector<int> puntajes;
pthread_mutex_t game_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t render_mutex = PTHREAD_MUTEX_INITIALIZER;

struct GhostArgs {
    Ghost* ghost;
    Mapa* mapa;
    Pacman* pacman;
    std::vector<Ghost*>* allGhosts;
    bool* running;
    pthread_mutex_t* game_mutex;
    pthread_mutex_t* render_mutex;
};

// FUNCION DE ENTRADA PARA EL HILO DE CADA FANTASMA
void* ghost_update(void* arg) {
    GhostArgs* args = static_cast<GhostArgs*>(arg);

    while (*args->running) {
        pthread_mutex_lock(args->game_mutex);
        args->ghost->update(*args->allGhosts, args->pacman);
        pthread_mutex_unlock(args->game_mutex);
        usleep(100);
    }

    return nullptr;
}

// FUNCION DE ENTRADA PARA EL HILO DEL FANTASMA CONTROLABLE
void* ghost_player_update(void* arg) {
    GhostArgs* args = static_cast<GhostArgs*>(arg);

    while (*args->running) {
        pthread_mutex_lock(args->game_mutex);
        args->ghost->update(*args->allGhosts, args->pacman);
        pthread_mutex_unlock(args->game_mutex);
    }

    return nullptr;
}

int gameLoop(int gameMode) {
    int frameCounter = 0;
    int ch = 0;
    nodelay(stdscr, TRUE);

    // Inicialización de objetos
    Mapa mapa;
    mapa.generarMapa(gameMode);

    // Calcular centro del mapa
    int cx = mapa.getAncho() / 2;
    int cy = mapa.getAlto() / 2;

    // Crear fantasmas en la caja central según el modo
    std::vector<Ghost*> fantasmas;
    Ghost* fantasma_controlable = nullptr;

    if (gameMode == 2) {
        fantasmas = {
            new Ghost(cx - 2, cy, ALEATORIO, mapa, 0),
            new Ghost(cx, cy, ALEATORIO, mapa, 1),
            new Ghost(cx + 2, cy, ALEATORIO, mapa, 2)
        };
        fantasma_controlable = new Ghost(cx, cy + 1, CONTROLABLE, mapa, 4);
    } else {
        fantasmas = {
            new Ghost(cx - 2, cy, ALEATORIO, mapa, 0),
            new Ghost(cx, cy, ALEATORIO, mapa, 1),
            new Ghost(cx + 2, cy, ALEATORIO, mapa, 2),
            new Ghost(cx, cy + 1, ALEATORIO, mapa, 3)
        };
    }

    // Pacman fuera de la caja, más abajo
    Pacman pacman(cx, cy + 6, mapa);

    // Información de modalidad
    std::vector<std::string> modeNames = {"CLASICO", "RAPIDO", "2 JUGADORES"};

    // Variable de control para los hilos
    bool running = true;

    // Crear vector para hilos de fantasmas
    std::vector<pthread_t> ghostThreads(fantasmas.size());
    std::vector<GhostArgs> ghostArgs;

    // Incluir fantasma controlable en el vector para colisiones
    std::vector<Ghost*> allGhosts = fantasmas;
    if (fantasma_controlable) {
        allGhosts.push_back(fantasma_controlable);
    }

    // Configuración de hilos para fantasmas aleatorios
    for (size_t i = 0; i < fantasmas.size(); i++) {
        GhostArgs args;
        args.ghost = fantasmas[i];
        args.mapa = &mapa;
        args.pacman = &pacman;
        args.allGhosts = &allGhosts;
        args.running = &running;
        args.game_mutex = &game_mutex;
        args.render_mutex = &render_mutex;
        ghostArgs.push_back(args);
    }

    // Lanzar los hilos de fantasmas aleatorios
    for (size_t i = 0; i < fantasmas.size(); i++) {
        pthread_create(&ghostThreads[i], nullptr, ghost_update, &ghostArgs[i]);
    }

    // Lanzar hilo del fantasma controlable si existe
    pthread_t fantasma_controlable_thread;
    GhostArgs* controlableArgs = nullptr;

    if (fantasma_controlable) {
        controlableArgs = new GhostArgs{
            fantasma_controlable,
            &mapa,
            &pacman,
            &allGhosts,
            &running,
            &game_mutex,
            &render_mutex
        };
        pthread_create(&fantasma_controlable_thread, nullptr, ghost_player_update, controlableArgs);
    }

    // Game loop principal
    while (ch != 113 && ch != 81) { // 'q' o 'Q'
        frameCounter++;
        ch = getch();

        // Obtener dimensiones de pantalla DENTRO del loop
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);

        if (frameCounter == 5000) {
            clear();
            frameCounter = 0;
        }

        // Guardar posiciones anteriores de fantasmas
        std::vector<std::pair<int, int>> ghost_old_positions;
        pthread_mutex_lock(&game_mutex);
        for (auto f : allGhosts) {
            if (f) {
                ghost_old_positions.push_back({f->getX(), f->getY()});
            }
        }
        pthread_mutex_unlock(&game_mutex);

        // Render
        pthread_mutex_lock(&game_mutex);
        pthread_mutex_lock(&render_mutex);
        mapa.draw();
        pacman.draw();

        // Mostrar modo de juego
        mvprintw(maxY - 3, maxX - 20, "Modo: %s", modeNames[gameMode].c_str());

        // Dibujar todos los fantasmas
        for (auto f : allGhosts) {
            if (f) f->draw();
        }
        pthread_mutex_unlock(&render_mutex);
        pthread_mutex_unlock(&game_mutex);

        // Limpiar posiciones anteriores de fantasmas
        pthread_mutex_lock(&game_mutex);
        for (size_t i = 0; i < allGhosts.size() && i < ghost_old_positions.size(); i++) {
            if (allGhosts[i] &&
                (allGhosts[i]->getX() != ghost_old_positions[i].first ||
                 allGhosts[i]->getY() != ghost_old_positions[i].second)) {

                pthread_mutex_lock(&render_mutex);
                Object* prevObj = mapa.getObjectAt(ghost_old_positions[i].first, ghost_old_positions[i].second);
                char prevSprite = (prevObj ? prevObj->sprite : ' ');
                mvaddch(ghost_old_positions[i].second, ghost_old_positions[i].first, prevSprite);
                pthread_mutex_unlock(&render_mutex);
            }
        }
        pthread_mutex_unlock(&game_mutex);

        int oldX = pacman.getX();
        int oldY = pacman.getY();

        // Actualización de Pacman protegida por mutex
        pthread_mutex_lock(&game_mutex);
        pacman.update();
        pthread_mutex_unlock(&game_mutex);

        // Limpiar posición anterior de Pacman
        if (oldX != pacman.getX() || oldY != pacman.getY()) {
            pthread_mutex_lock(&render_mutex);
            Object* prevObj = mapa.getObjectAt(oldX, oldY);
            char prevSprite = (prevObj ? prevObj->sprite : ' ');
            mvaddch(oldY, oldX, prevSprite);
            pthread_mutex_unlock(&render_mutex);
        }

        // Colisiones de Pacman con objetos
        pthread_mutex_lock(&game_mutex);
        Object* obj = mapa.getObjectAt(pacman.getX(), pacman.getY());

        if (obj && dynamic_cast<Punto*>(obj)) {
            mapa.addScore(10);
            mapa.clearArea(pacman.getX(), pacman.getY());
        } else if (obj && dynamic_cast<PowerUp*>(obj)) {
            PowerUp* pu = dynamic_cast<PowerUp*>(obj);
            if (pu) {
                pu->activarEfecto();
                mapa.clearArea(pacman.getX(), pacman.getY());
            }
        }

        // Colisiones pacman vs fantasmas
        bool gameOver = false;
        for (auto f : allGhosts) {
            if (f && f->getX() == pacman.getX() && f->getY() == pacman.getY()) {
                mapa.loseLife();
                if (mapa.getVidas() <= 0) {
                    // Game over
                    puntajes.push_back(mapa.getScore());
                    ch = 'q';
                    gameOver = true;
                    break;
                } else {
                    // Resetear posiciones
                    int centerX = mapa.getAncho() / 2;
                    int centerY = mapa.getAlto() / 2;
                    pacman.setPos(centerX, centerY + 6);
                    
                    // Respawnear fantasmas
                    for (auto ghost : allGhosts) {
                        if (ghost) ghost->respawn();
                    }
                }
            }
        }
        pthread_mutex_unlock(&game_mutex);

        if (gameOver) {
            refresh();
            break;
        }

        refresh();
    }

    // Señal para detener los hilos
    running = false;

    // Esperar a que terminen los hilos
    for (size_t i = 0; i < fantasmas.size(); i++) {
        pthread_join(ghostThreads[i], nullptr);
    }

    if (fantasma_controlable) {
        pthread_join(fantasma_controlable_thread, nullptr);
        delete controlableArgs;
        delete fantasma_controlable;
    }

    for (auto& f : fantasmas) {
        delete f;
    }
    fantasmas.clear();

    nodelay(stdscr, FALSE);
    return mapa.getScore();
}

int main() {
    setupNcurses();
    
    while (true) {
        int menuChoice = drawMainMenu();

        if (menuChoice == 0) {
            int gameMode = drawGameModeMenu();

            if (gameMode == -1) {
                continue;
            }

            clear();
            int final_score = gameLoop(gameMode);
            handle_end_of_game(final_score);
        } else if (menuChoice == 1) {
            drawInstructions();
        } else if (menuChoice == 2) {
            display_highscore_screen();
        } else if (menuChoice == 3) {
            break;
        }
    }
    
    pthread_mutex_destroy(&game_mutex);
    pthread_mutex_destroy(&render_mutex);

    closeNcurses();
    return 0;
}