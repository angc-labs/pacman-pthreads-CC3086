/*
* Angel Gabriel Chavez Otzoy - 24248
 * 12 / 09 / 2025 - inicializacion de la clase
 */

#include "../headers/Pacman.h"
#include <ncurses.h>

// contructor
Pacman::Pacman(int x, int y) : Object(x, y, 'C'), velocidad(1) {}

void Pacman::moveInput() {
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

void Pacman::checkCollision() {
    // HINT: condicional para las colisiones
}
// actualizar posicion
void Pacman::update() {
    moveInput();
    checkCollision();
    // siguiente posicion
}
// getters
int Pacman::getVelocidad() const {
    return velocidad;
}