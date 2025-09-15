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
    moveDelay = 500;
    frameCounterDirection = 0;
    moveDirectionDelay = 25*moveDelay;
}

void Ghost::move() {
    if (frameCounterDirection >= moveDirectionDelay) {
        frameCounterDirection = 0;
        direction = rand() % 4;
    }
}

bool Ghost::checkCollision(int tempX, int tempY) const {
    return (
        tempX <= 0 ||
        tempX >= mapa.getAncho() - 1 ||
        tempY <= 0 ||
        tempY >= mapa.getAlto() - 1 ||
        mapa.isWall(tempX, tempY)
        );
}

bool Ghost::shouldMoveThisFrame() {
    // mover en frame si framecounter es mayor que el delay
    frameCounter++;
    frameCounterDirection++;

    if (frameCounter >= moveDelay) {
        frameCounter = 0;
        return true;
    }
    return false;
}

void Ghost::update() {
    if (!shouldMoveThisFrame()) {
        return;
    }

    move();

    int tempX = x;
    int tempY = y;

    switch(direction) {
        case 0: tempY -= velocidad; break; // UP
        case 1: tempY += velocidad; break; // DOWN
        case 2: tempX -= velocidad; break; // LEFT
        case 3: tempX += velocidad; break; // RIGHT
    }

    if (!checkCollision(tempX, tempY)) {
        x = tempX;
        y = tempY;
    }
}
void Ghost::setEstado(const std::string& nuevoEstado) {
    estado = nuevoEstado;
}

void Ghost::respawn() {
    // Implementar reaparición
    estado = "normal";
}

std::string Ghost::getEstado() const {
    return estado;
}

std::string Ghost::getTipo() const {
    return tipo;
}