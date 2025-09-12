/*
 * Angel Gabriel Chavez Otzoy
 * 24248
 * 12 / 09 / 2025
 */
#include <ncurses.h>
#include "./../headers/Object.h"

Object::Object(int x, int y, char sprite) : x(x), y(y), sprite(sprite) {
}

Object::~Object() {
}

void Object::draw() {
    mvaddch(y, x, sprite);
}

void Object::update() {
    // Implementación por defecto vacía
}

int Object::getX() const {
    return x;
}

int Object::getY() const {
    return y;
}

void Object::setPos(int newX, int newY) {
    x = newX;
    y = newY;
}