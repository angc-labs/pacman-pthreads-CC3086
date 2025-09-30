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

int gameLoop(int gameMode) {
    start_music();
    
    int frameCounter = 0;
    int ch = 0;
    nodelay(stdscr, TRUE);

    // inicializacion de objetos
    Mapa mapa;

    // generar el mapa
    mapa.generarMapa();

    std::vector<Ghost*> fantasmas = {
        new Ghost(4, 10, "1", mapa),
        new Ghost(4, 11, "2", mapa),
        new Ghost(4, 12, "3", mapa),
        new Ghost(4, 13, "4", mapa),
        new Ghost(4, 14, "5", mapa)
    };

    // Buscar posición inicial válida para Pac-Man
    int cx = mapa.getAncho() / 2;
    int cy = mapa.getAlto() / 2;
    auto [pacX, pacY] = mapa.findFreeCellNear(cx, cy);
    Pacman pacman(pacX, pacY, mapa);

    //Pacman pacman(15, 10, mapa);

    auto [gx1, gy1] = mapa.findFreeCellNear(cx, cy);
    auto [gx2, gy2] = mapa.findFreeCellNear(cx, cy);
    auto [gx3, gy3] = mapa.findFreeCellNear(cx, cy);

    /*std::vector<Ghost*> fantasmas = {
        new Ghost(gx1, gy1, "1", mapa),
        new Ghost(gx2, gy2, "2", mapa),
        new Ghost(gx3, gy3, "3", mapa)
    };*/

    clear();

    // variable de control para los hilos
    bool running = true;

    // crear vector para hilos de fantasmas
    std::vector<pthread_t> ghostThreads(fantasmas.size());
    std::vector<GhostArgs> ghostArgs;

    // configuracion
    for (size_t i = 0; i < fantasmas.size(); i++) {
        GhostArgs args;
        args.ghost = fantasmas[i];
        args.mapa = &mapa;
        args.allGhosts = &fantasmas;
        args.running = &running;
        args.game_mutex = &game_mutex;
        ghostArgs.push_back(args);
    }

    // lanzar los hilos
    for (size_t i = 0; i < fantasmas.size(); i++) {
        pthread_create(&ghostThreads[i], nullptr, ghost_update, &ghostArgs[i]);
    }

    // bucle principal del juego
    while (ch != 'q' && ch != 'Q') {
        frameCounter++;
        ch = getch();
        if (frameCounter == 500) {
            clear();
        }

        // Control de música
        if (ch == 'm' || ch == 'M') {
            if (is_music_playing()) { // Si la música está sonando, detenerla
                stop_music();
            } else { // Si no está sonando, iniciarla
                start_music();
            }
        }

        bool any_fantasma_moved = false;

        pthread_mutex_lock(&game_mutex);
        for (const auto& fantasma : fantasmas) {
            int oldfantasmaX = fantasma->getX();
            int oldfantasmaY = fantasma->getY();

            fantasma->draw();
            fantasma->update(fantasmas);

            if (oldfantasmaX != fantasma->getX() || oldfantasmaY != fantasma->getY()) {
                Object* prevObj = mapa.getObjectAt(oldfantasmaX, oldfantasmaY);
                char prevSprite = (prevObj ? prevObj->sprite : ' ');
                // (y, x) = (fila, columna)
                mvaddch(oldfantasmaY, oldfantasmaX, prevSprite);
            }if (oldfantasmaX != fantasma->getX() || oldfantasmaY != fantasma->getY()) {
                any_fantasma_moved = true;
            }
        }
        pthread_mutex_unlock(&game_mutex);
        if (any_fantasma_moved) {
            // play_move_sound(); // Efecto de movimiento de fantasmas
        }

        // posiciones anteriores de fantasmas
        std::vector<std::pair<int, int>> ghost_old_ositions;
        pthread_mutex_lock(&game_mutex);
        for (auto f : fantasmas) {
            if (f) {
                ghost_old_ositions.push_back({f->getX(), f->getY()});
            }
        }
        pthread_mutex_unlock(&game_mutex);

        // render
        pthread_mutex_lock(&render_mutex);
        mapa.draw();
        pacman.draw();
        for (auto f : fantasmas) {
            if (f) f->draw();
        }
        pthread_mutex_unlock(&render_mutex);

        // limpiar posiciones anteriores de fantasmas
        pthread_mutex_lock(&game_mutex);
        for (size_t i = 0; i < fantasmas.size() && i < ghost_old_ositions.size(); i++) {
            if (fantasmas[i] &&
                (fantasmas[i]->getX() != ghost_old_ositions[i].first ||
                 fantasmas[i]->getY() != ghost_old_ositions[i].second)) {

                pthread_mutex_lock(&render_mutex);
                Object* prevObj = mapa.getObjectAt(ghost_old_ositions[i].first, ghost_old_ositions[i].second);
                char prevSprite = (prevObj ? prevObj->sprite : ' ');
                mvaddch(ghost_old_ositions[i].second, ghost_old_ositions[i].first, prevSprite);
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
            // play_eat_sound(); // Efecto de comer punto
        } else if (obj && dynamic_cast<PowerUp*>(obj)) {
            PowerUp* pu = dynamic_cast<PowerUp*>(obj);
            if (pu) {
                pu->activarEfecto();
                mapa.clearArea(pacman.getX(), pacman.getY());
                // play_powerup_sound();
            }
        }

        // colisiones pacman vs fantasmas
        for (auto f : fantasmas) {
            if (f && f->getX() == pacman.getX() && f->getY() == pacman.getY()) {
                mapa.loseLife();
                play_die_sound(); // Efecto de morir
                if (mapa.getVidas() <= 0) {
                    pthread_mutex_unlock(&game_mutex);
                    play_gameover_sound(); // Efecto de game over
                    refresh();
                    ch = 'q';
                    puntajes.push_back(mapa.getScore());
                    break;
                } else {
                    int cx = mapa.getAncho() / 2;
                    int cy = mapa.getAlto() / 2;
                    pacman.setPos(cx, cy);
                }
            }
        }

        if (ch != 'q') {
            pthread_mutex_unlock(&game_mutex);
        }

        refresh();
    }

    // señal para detener los hilos
    running = false;

    // esperando hilos
    for (size_t i = 0; i < fantasmas.size(); i++) {
        pthread_join(ghostThreads[i], nullptr);
    }

    for (auto& f : fantasmas) {
        delete f;
    }
    stop_music();
    fantasmas.clear();
    nodelay(stdscr, FALSE);
    return mapa.getScore();
}

/*int gameLoop() {
    start_music();

    int ch;
    nodelay(stdscr, TRUE);

    Mapa mapa;
    mapa.generarMapa(); // Generar el mapa ANTES de colocar entidades

    // Buscar posición inicial válida para Pac-Man
    int cx = mapa.getAncho() / 2;
    int cy = mapa.getAlto() / 2;
    auto [pacX, pacY] = mapa.findFreeCellNear(cx, cy);
    Pacman pacman(pacX, pacY, mapa);

    /*
    // Colocar fantasmas cerca del centro, pero en celdas libres
    int cx = mapa.getAncho() / 2;
    int cy = mapa.getAlto() / 2;
    std::vector<Ghost*> fantasmas = {
        new Ghost(cx, cy - 3, "1", mapa),
        new Ghost(cx - 1, cy - 3, "2", mapa),
        new Ghost(cx + 1, cy - 3, "3", mapa)
    };

    auto [gx1, gy1] = mapa.findFreeCellNear(cx, cy);
    auto [gx2, gy2] = mapa.findFreeCellNear(cx, cy);
    auto [gx3, gy3] = mapa.findFreeCellNear(cx, cy);

    std::vector<Ghost*> fantasmas = {
        new Ghost(gx1, gy1, "1", mapa),
        new Ghost(gx2, gy2, "2", mapa),
        new Ghost(gx3, gy3, "3", mapa)
    };

    clear();

    while (ch != 'q' && ch != 'Q') {
        ch = getch();

        if (ch == 'm' || ch == 'M') {
            if (is_music_playing()) stop_music();
            else start_music();
        }

        mapa.draw();
        pacman.draw();

        int oldpacX = pacman.getX();
        int oldpacY = pacman.getY();

        pacman.update();
        bool any_fantasma_moved = false;

        for (const auto& fantasma : fantasmas) {
            int oldfantasmaX = fantasma->getX();
            int oldfantasmaY = fantasma->getY();

            fantasma->draw();
            fantasma->update(fantasmas);

            if (oldfantasmaX != fantasma->getX() || oldfantasmaY != fantasma->getY()) {
                Object* prevObj = mapa.getObjectAt(oldfantasmaX, oldfantasmaY);
                char prevSprite = (prevObj ? prevObj->sprite : ' ');
                mvaddch(oldfantasmaY, oldfantasmaX, prevSprite);
                any_fantasma_moved = true;
            }
        }

        if (any_fantasma_moved) play_move_sound();

        int pacXnew = pacman.getX();
        int pacYnew = pacman.getY();

        if (oldpacX != pacXnew || oldpacY != pacYnew) {
            Object* prevObj = mapa.getObjectAt(oldpacX, oldpacY);
            char prevSprite = (prevObj ? prevObj->sprite : ' ');
            mvaddch(oldpacY, oldpacX, prevSprite);
        }

        Object* obj = mapa.getObjectAt(pacXnew, pacYnew);
        if (obj && dynamic_cast<Punto*>(obj)) {
            mapa.addScore(10);
            mapa.clearArea(pacXnew, pacYnew);
            play_eat_sound();
        } else if (obj && dynamic_cast<PowerUp*>(obj)) {
            PowerUp* pu = dynamic_cast<PowerUp*>(obj);
            pu->activarEfecto();
            mapa.clearArea(pacXnew, pacYnew);
            play_powerup_sound();
        }

        for (const auto& fantasma : fantasmas) {
            if (fantasma && fantasma->getX() == pacXnew && fantasma->getY() == pacYnew) {
                mapa.loseLife();
                play_die_sound();
                if (mapa.getVidas() <= 0) {
                    play_gameover_sound();
                    refresh();
                    ch = 'q';
                    puntajes.push_back(mapa.getScore());
                } else {
                    auto [respawnX, respawnY] = mapa.findFreeCellNear(cx, cy);
                    pacman.setPos(respawnX, respawnY);
                }
            }
        }

        refresh();
    }

    for (auto& fantasma : fantasmas) delete fantasma;
    fantasmas.clear();

    nodelay(stdscr, FALSE);
    return mapa.getScore();
}*/


int main() {
    setupNcurses();

    // bucle principal del menú
    while (true) {
        int menuChoice = drawMainMenu();

        if (menuChoice == 0) {
            int gameMode = drawGameModeMenu();

            if (gameMode == 3) {
                continue;
            }

            std::vector<std::string> modeNames = {
                "Modo Clasico",
                "Fantasmas Más Rápidos",
                "Dos Jugadores"
            };

            clear();
            int final_score = gameLoop();
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
