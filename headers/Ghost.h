/*
* Autor: Sarah Rachel Estrada Bonilla
* Fecha de creación: 12/9/2025
*/

#ifndef GHOST_H
#define CHOST_H

#include "Object.h"

class Ghost : public Object {
private:
    int velocidad;

public:
    Ghost(int x, int y);
    void moveInput();
    void checkCollision();
    void update() override;
    int getVelocidad() const;
};

#endif //GHOST_H
