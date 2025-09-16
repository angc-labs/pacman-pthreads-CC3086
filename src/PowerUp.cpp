/*
* Angel Gabriel Chavez Otzoy
 * 24248
 * 12 / 09 / 2025
 */

#include "./../headers/PowerUp.h"
#include <string>

// tipo de powerup
// HINT: actualizar a una mejor forma
// constructor
PowerUp::PowerUp(
    int x, int y, std::string t
    ) : Object(x, y, 'P'), tipo(t) {}

// funcion para activar efecto del powerup
void PowerUp::activarEfecto() {
        // HINT: efecto
}

std::string PowerUp::getTipo() const {
    return tipo;
}