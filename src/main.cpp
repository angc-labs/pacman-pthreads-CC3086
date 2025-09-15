#include <iostream>
#include <ncurses.h>
#include "./../headers/ui.h" // Para usar las funciones drawMainMenu
#include "./../headers/Mapa.h"
#include "./../headers/Pacman.h"
#include "./../headers/Punto.h"
#include <unistd.h> // Necesario para la función usleep()
#include <vector>
#include <memory>
#include "../headers/Ghost.h"

std::vector<int> puntajes;

void gameLoop() {
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
}

int main() {
    setupNcurses(); // Función auxiliar para configurar ncurses correctamente
    // Bucle principal que mantiene el programa en el menú
    while (true) {
        int menuChoice = drawMainMenu(); // Muestra el menú y espera elección de usuario

        if (menuChoice == 0) { // Opción de "Iniciar Juego"
            gameLoop(); // Lanzamiento del bucle del juego
        } else { // Opción de "Salir"
            break; // Termina el programa
        }
    }

    closeNcurses();
    return 0;
}
