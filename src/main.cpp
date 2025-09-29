#include <iostream>
#include <ncurses.h>
#include "ui.h" // Para usar las funciones drawMainMenu y de música
#include "Mapa.h"
#include "Pacman.h"
#include "Punto.h"
#include <unistd.h> // Necesario para la función usleep()
#include <vector>
#include <memory>
#include "Ghost.h"

std::vector<int> puntajes;

int gameLoop() {
    play_start_sound(); // Efecto de inicio

    int ch;

    nodelay(stdscr, TRUE);

    Mapa mapa;
    std::vector<Ghost*> fantasmas = {
        new Ghost(15, 10, "1", mapa),
        new Ghost(15, 11, "2", mapa),
        new Ghost(15, 12, "3", mapa)
    };
    Pacman pacman(15, 10, mapa);


    mapa.generarMapa();
    mapa.setVerticalLine(0.2 * mapa.getAncho(),0.5*mapa.getAlto(),mapa.getAlto());
    mapa.setHorizontalLine(0.5 * mapa.getAlto(),0.2*mapa.getAncho(),0.9*mapa.getAncho());

    clear(); // Limpia la pantalla antes de dibujar

    while (ch != 'q' && ch != 'Q') {
        ch = getch(); // Intenta obtener una tecla. Devuelve ERR si no hay

        // Control de música
        if (ch == 'm' || ch == 'M') {
            if (is_music_playing()) { // Si la música está sonando, detenerla
                stop_music();
            }else { // Si no está sonando, iniciarla
                start_music();
            }
        }

        mapa.draw();
        pacman.draw();

        int oldpacX = pacman.getX();
        int oldpacY = pacman.getY();

        pacman.update();
        for (const auto& fantasma : fantasmas) {
            int oldfantasmaX = fantasma->getX();
            int oldfantasmaY = fantasma->getY();

            fantasma->draw();
            fantasma->update(fantasmas);

            if (oldfantasmaX != fantasma->getX() || oldfantasmaY != fantasma->getY()) {
                play_move_sound(); // Efecto de movimiento de fantasmas
                Object* prevObj = mapa.getObjectAt(oldfantasmaX, oldfantasmaY);
                char prevSprite = (prevObj ? prevObj->sprite : ' ');
                // (y, x) = (fila, columna)
                mvaddch(oldfantasmaY, oldfantasmaX, prevSprite);
            }
        }

        int pacX = pacman.getX();
        int pacY = pacman.getY();

        if (oldpacX != pacX || oldpacY != pacY) {
            Object* prevObj = mapa.getObjectAt(oldpacX, oldpacY);
            char prevSprite = (prevObj ? prevObj->sprite : ' ');

            // (y, x) = (fila, columna)
            mvaddch(oldpacY, oldpacX, prevSprite);
        }

        Object* obj = mapa.getObjectAt(pacX, pacY);
        if (obj && dynamic_cast<Punto*>(obj)) {
            mapa.addScore(10);
            mapa.clearArea(pacX, pacY);
            play_eat_sound(); // Efecto de comer punto
        } else if (obj && dynamic_cast<PowerUp*>(obj)) {
            PowerUp* pu = dynamic_cast<PowerUp*>(obj);
            pu->activarEfecto();
            mapa.clearArea(pacX, pacY);
            play_powerup_sound(); // Efecto de power-up
        }

        for (const auto& fantasma : fantasmas) {
            if (fantasma && fantasma->getX() == pacX && fantasma->getY() == pacY) {
                mapa.loseLife();
                play_die_sound(); // Efecto de morir
                if (mapa.getVidas() <= 0) {
                    // game over
                    play_gameover_sound(); // Efecto de game over
                    refresh();
                    ch = 'q';
                    puntajes.push_back(mapa.getScore());
                } else {
                    // Resetear posiciones
                    int centerX = mapa.getAncho() / 2;
                    int centerY = mapa.getAlto() / 2;
                    pacman.setPos(centerX, centerY);
                }
            }
        }

        refresh();
    }
    for (auto& fantasma : fantasmas) {
        delete fantasma;
    }
    fantasmas.clear();
    nodelay(stdscr, FALSE);
    return mapa.getScore();
}

int main() {
    setupNcurses(); // Función auxiliar para configurar ncurses correctamente
    // Bucle principal que mantiene el programa en el menú
    while (true) {
        int menuChoice = drawMainMenu(); // Muestra el menú y espera elección de usuario

        if (menuChoice == 0) { // Opción de "Iniciar Juego"
            int final_score = gameLoop(); // Lanzamiento del bucle del juego
            handle_end_of_game(final_score);// Llamada a función para guardar puntaje
        } else if (menuChoice == 1) { // Opción de "Instrucciones"
            drawInstructions(); // Mostrar instrucciones
        } else if (menuChoice == 2) { // Opción de "Ver Puntajes"
            display_highscore_screen();
        }else if (menuChoice == 3) { // Opción de "Salir"
            break; // Termina el programa
        }
    }

    closeNcurses();
    return 0;
}
