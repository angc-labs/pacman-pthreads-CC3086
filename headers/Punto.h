/*
Angel Gabriel Chavez Otzoy
12/09/2025 - Inicializacinón de la clase
*/

#ifndef PACMAN_PTHREADS_CC3086_PUNTO_H
#define PACMAN_PTHREADS_CC3086_PUNTO_H
#include <string>
#include "Object.h"

#include "Object.h"

class Punto : public Object {
public:
    Punto(int x, int y);
    int getPuntos() const;
};


#endif //PACMAN_PTHREADS_CC3086_PUNTO_H