/*
 * Angel Gabriel Chavez Otzoy
 * 24248
 * 12 / 09 / 2025
 */
#include <ncursesw/ncurses.h>
#include "./../headers/Object.h"
#include <string>
#include <map>

Object::Object(int x, int y, char sprite) : x(x), y(y), sprite(sprite) {
}

Object::~Object() {
}

void Object::draw() {
    static std::map<char, const char*> emojis = {
        {'C', "💩"},
        {'G', "👻"},
        {'P', "⭐"}
    };

    // Aquí las claves son IDs de pares de colores, no colores directos
    static std::map<char, int> sprite_pairs = {
        {'#', 3},
        {'.', 2}
    };

    auto it = emojis.find(sprite);
    auto it_colors = sprite_pairs.find(sprite);

    if (it != emojis.end()) {
        mvprintw(y, x, "%s", it->second);
    } else if (it_colors != sprite_pairs.end()) {
        attron(COLOR_PAIR(it_colors->second));
        mvaddch(y, x, sprite);
        attroff(COLOR_PAIR(it_colors->second));
    } else {
        mvaddch(y, x, sprite);
    }
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