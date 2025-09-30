/*
* Autor: Sarah Rachel Estrada Bonilla
* Fecha de creación: 12/9/2025
*/

#include "../headers/Ghost.h"
#include "../headers/Object.h"
#include <ncursesw/curses.h>
#include <cstdlib>
#include <string>
#include <ctime>

Ghost::Ghost(int x, int y, GhostType t, Mapa& mapa) :
Object(
    x, y,
    (t == CONTROLABLE) ? 'M' : 'G'),
mapa(mapa) {
    srand(time(nullptr) + x + y);
    velocidad = 1;
    frameCounter = 0;
    tipo = t;
    estado = "normal";
    direction = 0;
    moveDelay = (t == CONTROLABLE) ? 500 : 100;
    frameCounterDirection = 0;
    moveDirectionDelay = 25*moveDelay;
    lastKey = 0;
}

void Ghost::moveInput() {}

void Ghost::move() {
    if (frameCounterDirection >= moveDirectionDelay) {
        frameCounterDirection = 0;
        direction = rand() % 4;
    }
}

bool Ghost::checkCollision(int tempX, int tempY, const std::vector<Ghost*>& ghosts) const {
    if (
    tempX <= 0 ||
    tempX >= mapa.getAncho() - 2 ||
    tempY <= 0 ||
    tempY >= mapa.getAlto() - 1 ||
    mapa.isWall(tempX+1, tempY) ||
    mapa.isWall(tempX, tempY)
) {
        return true;
    }

    for (const auto& fantasma : ghosts) {
        if (fantasma && fantasma != this &&
            fantasma->getX() == tempX && fantasma->getY() == tempY) {
            return true;
            }
    }

    return false;
}

bool Ghost::shouldMoveThisFrame() {
    frameCounter++;

    if (tipo == CONTROLABLE) {
        // Para el fantasma controlable, leer input directamente
        int currentKey = getch();
        if (currentKey != ERR) {
            if (currentKey == 87 || currentKey == 119 || // W
                currentKey == 83 || currentKey == 115 || // S
                currentKey == 65 || currentKey == 97 ||  // A
                currentKey == 68 || currentKey == 100) { // D
                lastKey = currentKey;
                frameCounter = 0;
                return true;
            } else {
                ungetch(currentKey);
            }
        }
        if (frameCounter >= moveDelay) {
            frameCounter = 0;
            return true;
        }
    } else {
        frameCounterDirection++;
        if (frameCounter >= moveDelay) {
            frameCounter = 0;
            return true;
        }
    }

    return false;
}

void Ghost::update(const std::vector<Ghost*>& ghosts) {
    if (!shouldMoveThisFrame()) {
        return;
    }

    if (tipo == CONTROLABLE) {
        moveInput();
    } else {
        move();
    }

    int tempX = x;
    int tempY = y;

    if (tipo == CONTROLABLE) {
        if (lastKey == 87 || lastKey == 119) { // W
            tempY -= velocidad;
        } else if (lastKey == 83 || lastKey == 115) { // S
            tempY += velocidad;
        } else if (lastKey == 65 || lastKey == 97) { // A
            tempX -= velocidad;
        } else if (lastKey == 68 || lastKey == 100) { // D
            tempX += velocidad;
        }
    } else {
        if (direction == 0) {
            tempY -= velocidad;
        } else if (direction == 1) {
            tempY += velocidad;
        } else if (direction == 2) {
            tempX -= velocidad;
        } else if (direction == 3) {
            tempX += velocidad;
        }
    }

    if (!checkCollision(tempX, tempY, ghosts)) {
        x = tempX;
        y = tempY;
    } else if (tipo == ALEATORIO) {
        direction = rand() % 4;
        frameCounterDirection = 0;
    }
}

void Ghost::setEstado(const std::string& nuevoEstado) {
    estado = nuevoEstado;
}

void Ghost::respawn() {
    // respawn
    estado = "normal";
}

std::string Ghost::getEstado() const {
    return estado;
}

GhostType Ghost::getTipo() const {
    return tipo;
}