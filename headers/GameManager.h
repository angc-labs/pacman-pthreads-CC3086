//
// Created by angco on 9/14/2025.
//

#ifndef PACMAN_PTHREADS_CC3086_GAMEMANAGER_H
#define PACMAN_PTHREADS_CC3086_GAMEMANAGER_H


#include "Mapa.h"
#include "Pacman.h"
// #include "Fantasma.h"
#include <memory>
#include <vector>
#include <string>

class GameManager {
private:
    std::vector<Object> Mapa;
public:
    GameManager();
    ~GameManager();

    void run();
};

#endif //PACMAN_PTHREADS_CC3086_GAMEMANAGER_H