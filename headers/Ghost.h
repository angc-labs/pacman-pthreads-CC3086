/*
* Autor: Sarah Rachel Estrada Bonilla
* Fecha de creación: 12/9/2025
*/

#ifndef GHOST_H
#define CHOST_H

#include "Object.h"
#include "Mapa.h"
#include <cstdlib>
#include <string>
#include <ctime>

class Ghost : public Object {
private:
    int velocidad;
    std::string tipo;
    std::string estado;
    Mapa& mapa;
    int direction;
    int frameCounter;
    int moveDelay;
    int frameCounterDirection;
    int moveDirectionDelay;

public:
    Ghost(int x, int y, std::string t, Mapa& mapa);
    void setEstado(const std::string& nuevoEstado);
    void respawn();
    void update(const std::vector<Ghost*>& ghosts);
    std::string getEstado() const;
    std::string getTipo() const;

private:
    bool checkCollision(int tempX, int tempY, const std::vector<Ghost*>& ghosts) const;
    void move();
    bool shouldMoveThisFrame();

};

#endif //GHOST_H
