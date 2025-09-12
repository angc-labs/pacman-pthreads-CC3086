/*
* Autor: Sarah Rachel Estrada Bonilla
* Fecha de creación: 12/9/2025
*/

#include "../headers/Ghost.h"
#include <ncurses.h>

// contructor
Ghost::Ghost(int x, int y) : Object(x, y, 'C'), velocidad(1) {}

// si hay dos jugadores, este metodo se puede usar para mover al fantasma
void Ghost::moveInput() {
    int key = getch();

    if (key == KEY_UP) {
        y -= velocidad;
    } else if (key == KEY_DOWN) {
        y += velocidad;
    } else if (key == KEY_LEFT) {
        x -= velocidad;
    } else if (key == KEY_RIGHT) {
        x += velocidad;
    }
}

void Ghost::checkCollision() {
    // HINT: condicional para las colisiones
}
// actualizar posicion
void Ghost::update() {
    moveInput();
    checkCollision();
    // siguiente posicion
}
// getters
int Ghost::getVelocidad() const {
    return velocidad;
}