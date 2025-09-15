#include <iostream>
#include <ncurses.h>
#include "./../headers/ui.h" // Para usar las funciones drawMainMenu
#include "./../headers/Mapa.h"
#include "./../headers/Pacman.h"
#include "./../headers/Punto.h"
#include <unistd.h> // Necesario para la función usleep()
#include <memory>

#include "../headers/Ghost.h"

void gameLoop() {
    int ch; // Almacena la tecla presionada
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
    while (true) {
        ch = getch(); // Intenta obtener una tecla. Devuelve ERR si no hay

        // Si se presiona 'q', sale del bucle del juego
        if (ch == 'q' || ch == 'Q'){
            break;
        }

        clear();
        mapa.draw();
        pacman.draw();

        pacman.update();
        for (const auto& fantasma : fantasmas) {
            fantasma->draw();
            fantasma->update();
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

    closeNcurses(); // Restauración de configuración original de la terminal
    return 0;
}
