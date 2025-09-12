/*
* Angel Gabriel Chavez Otzoy
 * 24248
 * 12 / 09 / 2025
 */

#include "./../headers/Object.h"
#include <string>

class PowerUp : public Object {
// tipo de powerup
// HINT: actualizar a una mejor forma
private:
    std::string tipo;

public:
    // constructor
    PowerUp(int x, int y, std::string t) : Object(x, y, 'O'), tipo(t) {}

    // funcion para activar efecto del powerup
    void activarEfecto() {
        // HINT: efecto
    }
};