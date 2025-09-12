//
// Created by angc__ on 9/12/25.
//

#ifndef POWERUP_H
#define POWERUP_H

#include "Object.h"
#include <string>

class PowerUp : public Object {
private:
    std::string tipo;

public:
    PowerUp(int x, int y, std::string t);
    void activarEfecto();
};

#endif //POWERUP_H
