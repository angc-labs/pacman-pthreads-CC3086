/*
Angel Gabriel Chavex Otzoy
14/09/2025
*/
#include "./../headers/Pared.h"
#include <string>

Pared::Pared(int x, int y, std::string c) : Object(x, y, '#'), color(c) {}

std::string Pared::getColor() const {
    return color;
}

void Pared::setColor(const std::string& newColor) {
    color = newColor;
}