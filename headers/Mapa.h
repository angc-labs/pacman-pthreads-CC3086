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
    std::vector<std::vector<Object*>> mapa;
    int score, vidas, ancho, alto;
    std::vector<PowerUp*> powerups;

    // Métodos privados para generar mapas específicos
    void generarMapaClasico();
    void generarMapaRapido();
    void generarMapaDosJugadores();

public:
    Mapa();
    ~Mapa();

    // Ahora acepta un parámetro de modalidad (0=Clásico, 1=Rápido, 2=Dos Jugadores)
    void generarMapa(int modalidad = 0);
    
    void setPuente(int y, int size);
    void setXPuente(int x, int size);
    void setVerticalLine(int x, int startY, int endY);
    void setHorizontalLine(int y, int startX, int endX);
    Object* getObjectAt(int x, int y) const;
    bool isWall(int x, int y) const;
    std::pair<int, int> findFreeCellNear(int cx, int cy) const;

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
    void clearArea(int x, int y);
};

#endif // MAPA_H