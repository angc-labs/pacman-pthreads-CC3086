/*
* Angel Gabriel Chavez Otzoy - 24248
 * 12 / 09 / 2025 - inicializacion de la clase
 */

#ifndef PACMAN_H
#define PACMAN_H

#include "../headers/Object.h"
#include "../headers/Mapa.h"
#include <ncurses.h>

class Pacman : public Object {
private:
    int velocidad;
    int lastKey;
    Mapa& mapa;
    int frameCounter;
    int moveDelay;

public:
    Pacman(int x, int y, Mapa& map);
    void update() override;
    int getVelocidad() const;
    int getLastKey() const;
    void setMoveDelay(int delay);

private:
    void moveInput();
    bool checkCollision(int tempX, int tempY) const;
    bool shouldMoveThisFrame();
};

#endif