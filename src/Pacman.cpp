/*
* Angel Gabriel Chavez Otzoy - 24248
 * 12 / 09 / 2025 - inicializacion de la clase
 */

#include "../headers/Pacman.h"
#include <ncurses.h>

Pacman::Pacman(int x, int y, Mapa& map) :
    Object(x, y, 'C'),
    velocidad(1),
    lastKey(0),
    mapa(map),
    frameCounter(0),
    moveDelay(1000) {}

void Pacman::moveInput() {
    int key = getch();
    if (key != ERR) {
        lastKey = key;
    }
}

bool Pacman::checkCollision(int tempX, int tempY) const {
    return (
        tempX <= 0 ||
        tempX >= mapa.getAncho() - 2 ||
        tempY <= 0 ||
        tempY >= mapa.getAlto() - 1 ||
        mapa.isWall(tempX+1, tempY) ||
        mapa.isWall(tempX, tempY)
        );
}

bool Pacman::shouldMoveThisFrame() {
    // mover en trame si framecounter es mayor que el delay
    // mover si hay una tecla presionada
    frameCounter++;

    int currentKey = getch();
    if (currentKey != ERR && currentKey != lastKey) {
        ungetch(currentKey);
        return true;
    }

    if (frameCounter >= moveDelay) {
        frameCounter = 0;
        return true;
    }
    return false;
}

void Pacman::update() {
    if (!shouldMoveThisFrame()) {
        return;
    }

    moveInput();
    if (lastKey == 0) {
        return;
    }

    // posicion basada en lastKey
    int tempX = x;
    int tempY = y;

    if (lastKey == KEY_UP) {
        tempY -= velocidad;
    } else if (lastKey == KEY_DOWN) {
        tempY += velocidad;
    } else if (lastKey == KEY_LEFT) {
        tempX -= velocidad;
    } else if (lastKey == KEY_RIGHT) {
        tempX += velocidad;
    }

    if (!checkCollision(tempX, tempY)) {
        x = tempX;
        y = tempY;
    }
}

int Pacman::getVelocidad() const {
    return velocidad;
}

int Pacman::getLastKey() const {
    return lastKey;
}

void Pacman::setMoveDelay(int delay) {
    if (delay > 0) {
        moveDelay = delay;
    } else {
        moveDelay = 1;
    }
}