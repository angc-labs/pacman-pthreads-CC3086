/*
* Angel Gabriel Chavez Otzoy - 24248
 * 12 / 09 / 2025 - inicializacion de la clase
 */

#ifndef PACMAN_H
#define PACMAN_H

#include "Object.h"

class Pacman : public Object {
private:
    int velocidad;

public:
    Pacman(int x, int y);
    void moveInput();
    void checkCollision();
    void update() override;
    int getVelocidad() const;
};

#endif //PACMAN_H
