#include <iostream>
#include <ncursesw/ncurses.h>
#include "../headers/ui.h"
#include "../headers/Mapa.h"
#include "../headers/Pacman.h"
#include "../headers/Punto.h"
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
        args->ghost->update(*args->allGhosts);
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
        args->ghost->update(*args->allGhosts);
        pthread_mutex_unlock(args->game_mutex);
    }

    return nullptr;
}

int gameLoop(int gameMode) {
    start_music();

    int frameCounter = 0;
    int ch = 0;
    nodelay(stdscr, TRUE);

    // inicializacion de objetos
    Mapa mapa;
    std::vector<Ghost*> fantasmas;
    Ghost* fantasma_controlable = nullptr;

    if (gameMode == 1) {
        fantasmas = {
            new Ghost(4, 10, ALEATORIO, mapa),
            new Ghost(4, 11, ALEATORIO, mapa),
            new Ghost(4, 12, ALEATORIO, mapa)
        };
        fantasma_controlable = new Ghost(4, 14, CONTROLABLE, mapa);
    } else if (gameMode == 0) {
        fantasmas = {
            new Ghost(4, 10, ALEATORIO, mapa),
            new Ghost(4, 11, ALEATORIO, mapa),
            new Ghost(4, 12, ALEATORIO, mapa),
            new Ghost(4, 13, ALEATORIO, mapa)
        };
    }

    Pacman pacman(15, 10, mapa);
    // ===================================================
    // GENERAR MAPA
    // ===================================================
    mapa.generarMapa();

    mapa.setHorizontalLine(0.25 * mapa.getAlto(), 0.1 * mapa.getAncho(), 0.4 * mapa.getAncho());
    mapa.setHorizontalLine(0.25 * mapa.getAlto(), 0.6 * mapa.getAncho(), 0.9 * mapa.getAncho());
    mapa.setHorizontalLine(0.5 * mapa.getAlto(), 0.2 * mapa.getAncho(), 0.8 * mapa.getAncho());
    mapa.setHorizontalLine(0.75 * mapa.getAlto(), 0.1 * mapa.getAncho(), 0.4 * mapa.getAncho());
    mapa.setHorizontalLine(0.75 * mapa.getAlto(), 0.6 * mapa.getAncho(), 0.9 * mapa.getAncho());

    mapa.clearArea(0.1 * mapa.getAncho(), 0.1 * mapa.getAlto());
    mapa.clearArea(0.2 * mapa.getAncho(), 0.1 * mapa.getAlto());
    mapa.clearArea(0.1 * mapa.getAncho(), 0.2 * mapa.getAlto());

    // esquina superior derecha
    mapa.clearArea(0.8 * mapa.getAncho(), 0.1 * mapa.getAlto());
    mapa.clearArea(0.9 * mapa.getAncho(), 0.1 * mapa.getAlto());
    mapa.clearArea(0.8 * mapa.getAncho(), 0.2 * mapa.getAlto());

    // esquina inferior izquierda
    mapa.clearArea(0.1 * mapa.getAncho(), 0.8 * mapa.getAlto());
    mapa.clearArea(0.2 * mapa.getAncho(), 0.8 * mapa.getAlto());
    mapa.clearArea(0.1 * mapa.getAncho(), 0.9 * mapa.getAlto());

    // esquina inferior derecha - area abierta
    mapa.clearArea(0.8 * mapa.getAncho(), 0.8 * mapa.getAlto());
    mapa.clearArea(0.9 * mapa.getAncho(), 0.8 * mapa.getAlto());
    mapa.clearArea(0.8 * mapa.getAncho(), 0.9 * mapa.getAlto());

    for (int x = 0.4 * mapa.getAncho(); x < 0.6 * mapa.getAncho(); x++) {
        for (int y = 0.4 * mapa.getAlto(); y < 0.6 * mapa.getAlto(); y++) {
            mapa.clearArea(x, y);
        }
    }

    // T vertical izquierda
    mapa.setVerticalLine(0.2 * mapa.getAncho(), 0.4 * mapa.getAlto(), 0.6 * mapa.getAlto());
    mapa.setHorizontalLine(0.4 * mapa.getAlto(), 0.2 * mapa.getAncho(), 0.35 * mapa.getAncho());

    // T vertical derecha
    mapa.setVerticalLine(0.8 * mapa.getAncho(), 0.4 * mapa.getAlto(), 0.6 * mapa.getAlto());
    mapa.setHorizontalLine(0.6 * mapa.getAlto(), 0.65 * mapa.getAncho(), 0.8 * mapa.getAncho());

    mapa.setPuente(0.3 * mapa.getAlto(), 3);
    mapa.setPuente(0.7 * mapa.getAlto(), 3);
    mapa.setXPuente(0.5 * mapa.getAncho(), 3);
    // ===================================================

    // variable de control para los hilos
    bool running = true;

    // crear vector para hilos de fantasmas
    std::vector<pthread_t> ghostThreads(fantasmas.size());
    std::vector<GhostArgs> ghostArgs;

    // incluir fantasma controlable en el vector para colisiones
    std::vector<Ghost*> allGhosts = fantasmas;
    if (fantasma_controlable) {
        allGhosts.push_back(fantasma_controlable);
    }

    // configuracion de hilos para fantasmas aleatorios
    for (size_t i = 0; i < fantasmas.size(); i++) {
        GhostArgs args;
        args.ghost = fantasmas[i];
        args.mapa = &mapa;
        args.allGhosts = &allGhosts;
        args.running = &running;
        args.game_mutex = &game_mutex;
        args.render_mutex = &render_mutex;
        ghostArgs.push_back(args);
    }

    // lanzar los hilos de fantasmas aleatorios
    for (size_t i = 0; i < fantasmas.size(); i++) {
        pthread_create(&ghostThreads[i], nullptr, ghost_update, &ghostArgs[i]);
    }

    pthread_t fantasma_controlable_thread;
    GhostArgs* controlableArgs = nullptr;

    if (fantasma_controlable) {
        controlableArgs = new GhostArgs{
            fantasma_controlable,
            &mapa,
            &allGhosts,
            &running,
            &game_mutex,
            &render_mutex
        };
        pthread_create(&fantasma_controlable_thread, nullptr, ghost_player_update, controlableArgs);
    }

    while (ch != 113 && ch != 81) {
        frameCounter++;
        ch = getch();

        if (frameCounter ==1000) {
            clear();
            frameCounter=0;
        }

        //=================================
        // Control de música
        if (ch == 'm' || ch == 'M') {
            if (is_music_playing()) {
                stop_music();
            } else {
                start_music();
            }
        }

        bool any_fantasma_moved = false;
        if (any_fantasma_moved) {
            // play_move_sound(); // Efecto de movimiento de fantasmas
        }
        // ==============================================

        // posiciones anteriores de fantasmas
        std::vector<std::pair<int, int>> ghost_old_positions;
        pthread_mutex_lock(&game_mutex);
        for (auto f : allGhosts) {
            if (f) {
                ghost_old_positions.push_back({f->getX(), f->getY()});
            }
        }
        pthread_mutex_unlock(&game_mutex);

        // render
        pthread_mutex_lock(&game_mutex);
        pthread_mutex_lock(&render_mutex);
        mapa.draw();
        pacman.draw();

        // Dibujar todos los fantasmas
        for (auto f : allGhosts) {
            if (f) f->draw();
        }
        pthread_mutex_unlock(&render_mutex);
        pthread_mutex_unlock(&game_mutex);

        // limpiar posiciones anteriores de fantasmas
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

        // actualización de Pacman protegida por mutex
        pthread_mutex_lock(&game_mutex);
        pacman.update();
        pthread_mutex_unlock(&game_mutex);

        // limpiar posición anterior de Pacman
        if (oldX != pacman.getX() || oldY != pacman.getY()) {
            pthread_mutex_lock(&render_mutex);
            Object* prevObj = mapa.getObjectAt(oldX, oldY);
            char prevSprite = (prevObj ? prevObj->sprite : ' ');
            mvaddch(oldY, oldX, prevSprite);
            pthread_mutex_unlock(&render_mutex);
        }

        // colisiones de Pacman con objetos
        pthread_mutex_lock(&game_mutex);
        Object* obj = mapa.getObjectAt(pacman.getX(), pacman.getY());

        if (obj && dynamic_cast<Punto*>(obj)) {
            mapa.addScore(10);
            mapa.clearArea(pacman.getX(), pacman.getY());
        } else if (obj && dynamic_cast<PowerUp*>(obj)) {
            PowerUp* pu = dynamic_cast<PowerUp*>(obj);
            if (pu) {
                pu->activarEfecto();
                mapa.addScore(200);
                mapa.clearArea(pacman.getX(), pacman.getY());
            }
        }

        // colisiones pacman vs fantasmas
        bool gameOver = false;
        for (auto f : allGhosts) {
            if (f && f->getX() == pacman.getX() && f->getY() == pacman.getY()) {
                mapa.loseLife();
                play_die_sound(); // Efecto de morir
                if (mapa.getVidas() <= 0) {
                    puntajes.push_back(mapa.getScore());
                    pthread_mutex_unlock(&game_mutex);
                    play_gameover_sound(); // Efecto de game over
                    refresh();
                    ch = 'q';
                    gameOver = true;
                    break;
                } else {
                    int cx = mapa.getAncho() / 2;
                    int cy = mapa.getAlto() / 2;
                    pacman.setPos(cx, cy);
                }
            }
        }

        pthread_mutex_unlock(&game_mutex);

        if (gameOver) {
            refresh();
        }

        refresh();
    }

    // señal para detener los hilos
    running = false;

    // esperando hilos
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
    stop_music();
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

            if (gameMode == 3 || gameMode == 2) {
                continue;
            }

            std::vector<std::string> modeNames = {
                "Modo clasico",
                "Dos jugadores"
            };

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
