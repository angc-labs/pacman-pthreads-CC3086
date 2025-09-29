/*
* Autor: Sarah Rachel Estrada Bonilla
* Fecha de creación: 12/9/2025
*/

#include "../headers/Ghost.h"
#include "../headers/Object.h"
#include <cstdlib>
#include <string>
#include <ctime>

Ghost::Ghost(int x, int y, std::string t, Mapa& mapa) :
Object(x, y, 'G'), mapa(mapa){
    srand(time(nullptr));
    velocidad = 1;
    frameCounter = 0;
    tipo = t;
    estado = "normal";
    direction = 0;
    moveDelay = 1000;
    frameCounterDirection = 0;
    moveDirectionDelay = 25*moveDelay;
}

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
    // mover en frame si framecounter
    // es mayor que el delay
    frameCounter++;
    frameCounterDirection++;

    if (frameCounter >= moveDelay) {
        frameCounter = 0;
        return true;
    }
    return false;
}

void Ghost::update(const std::vector<Ghost*>& ghosts) {
    if (!shouldMoveThisFrame()) {
        return;
    }
    move();
    int tempX = x;
    int tempY = y;

    if (direction == 0) {
        tempY -= velocidad;
    } else if (direction == 1) {
        tempY += velocidad;
    } else if (direction == 2) {
        tempX -= velocidad;
    } else if (direction == 3) {
        tempX += velocidad;
    }

    if (!checkCollision(tempX, tempY, ghosts)) {
        x = tempX;
        y = tempY;
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

std::string Ghost::getTipo() const {
    return tipo;
}