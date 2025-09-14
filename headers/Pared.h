/*
Angel Gabriel Chavez Otzoy
14 / 09 / 2025
*/

#ifndef PACMAN_PTHREADS_CC3086_PARED_H
#define PACMAN_PTHREADS_CC3086_PARED_H
#include "Object.h"
#include <string>

class Pared : public Object {
private:
    std::string color;

public:
    Pared(int x, int y, std::string c = "blue");
    std::string getColor() const;
    void setColor(const std::string& newColor);
};

#endif //PACMAN_PTHREADS_CC3086_PARED_H