#include <iostream>
#include <ncurses.h>
#include "ui.h"
#include "Mapa.h"
#include "Pacman.h"
#include "Punto.h"
#include <unistd.h>
#include <vector>
#include <memory>
#include "Ghost.h"

std::vector<int> puntajes;

int gameLoop(int gameMode) {
    int ch;
    nodelay(stdscr, TRUE);

    Mapa mapa;
    
    mapa.generarMapa(gameMode);
    
    int cx = mapa.getAncho() / 2;
    int cy = mapa.getAlto() / 2;
    
    // Fantasmas dentro de la caja central
    std::vector<Ghost*> fantasmas = {
        new Ghost(cx - 2, cy, "1", mapa),
        new Ghost(cx, cy, "2", mapa),
        new Ghost(cx + 2, cy, "3", mapa)
    };
    
    // Pacman fuera de la caja, abajo
    Pacman pacman(cx, cy + 6, mapa);

    // Ajustes según la modalidad seleccionada
    switch (gameMode) {
        case 0: // Modo Clásico
            // Configuración por defecto 
            break;
            
        case 1: // Fantasmas Más Rápidos
            for (auto& fantasma : fantasmas) {
                // Lógica para aumentar la velocidad de los fantasmas
                // Puedes ajustar el moveDelay en Ghost si lo haces accesible
            }
            break;
            
        case 2: // Dos Jugadores
            // Implementar lógica para el segundo jugador
            break;
    }
    
    // mapa.setVerticalLine(0.2 * mapa.getAncho(), 0.5 * mapa.getAlto(), mapa.getAlto());
    // mapa.setHorizontalLine(0.5 * mapa.getAlto(), 0.2 * mapa.getAncho(), 0.9 * mapa.getAncho());
    
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

    closeNcurses();
    return 0;
}