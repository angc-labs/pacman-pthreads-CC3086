//
// Created by angco on 9/14/2025.
//

#ifndef PACMAN_PTHREADS_CC3086_MAPA_H
#define PACMAN_PTHREADS_CC3086_MAPA_H

#include <vector>
#include "Object.h"
#include "PowerUp.h"
#include "Punto.h"
#include "Pared.h"
#include <cstdlib>
#include <ctime>

class Pared;
class Punto;

class Mapa {
private:
    std::vector<std::vector<Object*>> mapa;        // Vector para almacenar punteros a objetos
    int score, vidas, ancho, alto;    // Puntuación, vidas, dimensiones del mapa
    std::vector<PowerUp*> powerups;   // Vector para almacenar power-ups

public:
    Mapa();
    ~Mapa();

    void generarMapa();
    void setPuente(int y, int size);
    void setXPuente(int x, int size);
    void setVerticalLine(int x, int startY, int endY);
    void setHorizontalLine(int y, int startX, int endX);
    Object* getObjectAt(int x, int y);
    bool isWall(int x, int y);

    // Getters y setters
    int getScore() const;
    int getVidas() const;
    int getAncho() const;
    int getAlto() const;
    void setScore(int newScore);
    void setVidas(int newVidas);

    // Métodos de juego
    void draw();
    void addScore(int puntos);
    void loseLife();

    // limpiar area especifica
    void clearArea(int x, int y);
};

#endif // MAPA_H