#include <iostream>
#include <ncurses.h>
#include "ui.h" // Para usar las funciones drawMainMenu
#include "Mapa.h"
#include "Pacman.h"
#include "Punto.h"
#include <unistd.h> // Necesario para la función usleep()
#include <vector>
#include <memory>
#include "Ghost.h"

std::vector<int> puntajes;

int gameLoop() {
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
    while (ch != 'q' && ch != 'Q') {
        ch = getch(); // Intenta obtener una tecla. Devuelve ERR si no hay

        clear();
        mapa.draw();
        pacman.draw();

        pacman.update();
        for (const auto& fantasma : fantasmas) {
            fantasma->draw();
            fantasma->update(fantasmas);
        }

        int pacX = pacman.getX();
        int pacY = pacman.getY();

        Object* obj = mapa.getObjectAt(pacX, pacY);
        if (obj && dynamic_cast<Punto*>(obj)) {
            mapa.addScore(10);
            mapa.clearArea(pacX, pacY);
        } else if (obj && dynamic_cast<PowerUp*>(obj)) {
            PowerUp* pu = dynamic_cast<PowerUp*>(obj);
            pu->activarEfecto();
            mapa.clearArea(pacX, pacY);
        }

        for (const auto& fantasma : fantasmas) {
            if (fantasma && fantasma->getX() == pacX && fantasma->getY() == pacY) {
                mapa.loseLife();
                if (mapa.getVidas() <= 0) {
                    // game over
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
        usleep(5);
    }
    for (auto& fantasma : fantasmas) {
        delete fantasma;
    }
    fantasmas.clear();
    nodelay(stdscr, FALSE);
    return mapa.getScore(); // Devolución de puntaje final
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
