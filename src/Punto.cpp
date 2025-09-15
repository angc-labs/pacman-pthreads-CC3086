/*
Angel Gabriel Chavez Otzoy
12/09/2025 - Inicializacinón de la clase
*/

#include "./../headers/Punto.h"

Punto::Punto(int x, int y) : Object(x, y, '.') {}

int Punto::getPuntos() const {
    return 10;
}