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

int gameLoop(int gameMode) {
    int ch;

    nodelay(stdscr, TRUE);

    Mapa mapa;
    std::vector<Ghost*> fantasmas = {
        new Ghost(15, 10, "1", mapa),
        new Ghost(15, 11, "2", mapa),
        new Ghost(15, 12, "3", mapa)
    };
    Pacman pacman(15, 10, mapa);

    /*
    * Ajustes según la modalidad seleccionada
    */
    switch (gameMode) {
        case 0: // Modo Clásico
            // Configuración por defecto 
            break;
            
        case 1: // Fantasmas Más Rápidos
            for (auto& fantasma : fantasmas) {
                // Agredar lógica para aumentar la velocidad de los fantasmas
            }
            break;
            
        case 2: // Dos Jugadores
            // Aquí implementarías la lógica para el segundo jugador
            break;
    }

    mapa.generarMapa();
    mapa.setVerticalLine(0.2 * mapa.getAncho(), 0.5 * mapa.getAlto(), mapa.getAlto());
    mapa.setHorizontalLine(0.5 * mapa.getAlto(), 0.2 * mapa.getAncho(), 0.9 * mapa.getAncho());
    
    // Mostrar información de la modalidad activa
    std::vector<std::string> modeNames = {"CLASICO", "RAPIDO", "2 JUGADORES"};
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    while (ch != 'q' && ch != 'Q') {
        ch = getch();

        mapa.draw();
        pacman.draw();

        mvprintw(maxY - 3, maxX - 20, "Modo: %s", modeNames[gameMode].c_str());

        int oldpacX = pacman.getX();
        int oldpacY = pacman.getY();

        pacman.update();
        for (const auto& fantasma : fantasmas) {
            int oldfantasmaX = fantasma->getX();
            int oldfantasmaY = fantasma->getY();

            fantasma->draw();
            fantasma->update(fantasmas);

            if (oldfantasmaX != fantasma->getX() || oldfantasmaY != fantasma->getY()) {
                Object* prevObj = mapa.getObjectAt(oldfantasmaX, oldfantasmaY);
                char prevSprite = (prevObj ? prevObj->sprite : ' ');
                mvaddch(oldfantasmaY, oldfantasmaX, prevSprite);
            }
        }

        int pacX = pacman.getX();
        int pacY = pacman.getY();

        if (oldpacX != pacX || oldpacY != pacY) {
            Object* prevObj = mapa.getObjectAt(oldpacX, oldpacY);
            char prevSprite = (prevObj ? prevObj->sprite : ' ');
            mvaddch(oldpacY, oldpacX, prevSprite);
        }

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
                    refresh();
                    ch = 'q';
                    puntajes.push_back(mapa.getScore());
                } else {
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
    setupNcurses();
    
    while (true) {
        int menuChoice = drawMainMenu();

        if (menuChoice == 0) { // Opción de "Iniciar Juego"
            // PRIMERO mostrar selección de modalidad
            int gameMode = drawGameModeMenu();
            
            if (gameMode == 3) { // Si seleccionó "Volver"
                continue; // Volver al menú principal sin iniciar juego
            }
            
            // Aquí puedes mostrar información de la modalidad seleccionada
            std::vector<std::string> modeNames = {
                "Modo Clasico", 
                "Fantasmas Más Rápidos", 
                "Dos Jugadores"
            };
            
            clear();

            
            // Iniciar el juego con la modalidad seleccionada
            int final_score = gameLoop(gameMode);
            handle_end_of_game(final_score);
            
        } else if (menuChoice == 1) { // Opción de "Instrucciones"
            drawInstructions();
        } else if (menuChoice == 2) { // Opción de "Ver Puntajes"
            display_highscore_screen();
        } else if (menuChoice == 3) { // Opción de "Salir"
            break;
        }
    }

    closeNcurses();
    return 0;
}