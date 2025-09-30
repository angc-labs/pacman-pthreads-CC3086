/*
 * Autor: Sarah Rachel Estrada Bonilla
 * Fecha de creación: 12/9/2025
 */

#ifndef GHOST_H
#define GHOST_H

#include "Object.h"
#include "Mapa.h"
#include "Pacman.h"
#include <cstdlib>
#include <string>
#include <ctime>
#include <vector>

enum GhostType {
    ALEATORIO,
    CONTROLABLE
};

enum GhostState {
    IN_BOX,
    EXITING_BOX,
    CHASE,
    SCATTER,
    FRIGHTENED
};

class Ghost : public Object {
private:
    int velocidad;
    int lastKey;
    GhostType tipo;
    GhostState estado;
    Mapa& mapa;
    int direction;
    int frameCounter;
    int moveDelay;
    int frameCounterDirection;
    int moveDirectionDelay;
    int ghostID;
    int releaseTimer;
    std::pair<int, int> targetCorner;
    std::pair<int, int> spawnPoint;

public:
    Ghost(int x, int y, GhostType t, Mapa& mapa, int id = 0);
    void update(const std::vector<Ghost*>& ghosts, Pacman* pacman);
    void setEstado(GhostState nuevoEstado);
    void respawn();
    GhostState getEstado() const;
    GhostType getTipo() const;
    void setReleaseTimer(int frames);

private:
    bool checkCollision(int tempX, int tempY, const std::vector<Ghost*>& ghosts) const;
    void moveAI(Pacman* pacman);
    void moveInput();
    bool shouldMoveThisFrame();
    int getDirectionToTarget(int targetX, int targetY);
    void exitBox();
    std::pair<int, int> getChaseTarget(Pacman* pacman);
    std::pair<int, int> getScatterTarget();
    std::pair<int, int> getFrightenedTarget(Pacman* pacman);
};

#endif