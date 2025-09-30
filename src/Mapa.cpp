/*
* Universidad del Valle de Guatemala
* PROGRAMACIÓN DE MICROPROCESADORES
* Sección: 20
* Autora: Tiffany Salazar Suarez
* Carnét: 24630
*/

#include "../headers/Mapa.h"
#include "../headers/Pared.h"
#include "../headers/Punto.h"
#include "../headers/PowerUp.h"
#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <bits/ostream.tcc>

Mapa::Mapa() : score(0), vidas(4) {
    // Obtener dimensiones automáticamente
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    ancho = maxX;
    alto = maxY - 3;  // Reservar 3 líneas para información

    // Inicializar el mapa como matriz anidada
    mapa.resize(alto);
    for (int y = 0; y < alto; y++) {
        mapa[y].resize(ancho, nullptr);
    }
}

Mapa::~Mapa() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }

    // Liberar power-ups
    for (PowerUp* power : powerups) {
        delete power;
    }
    powerups.clear();
}

void Mapa::generarMapa() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }

    // Limpiar power-ups
    for (PowerUp* power : powerups) {
        delete power;
    }
    powerups.clear();

    // paredes del borde y algunos puntos
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (y == 0 || y == alto - 1 || x == 0 || x == ancho - 1) {
                // Paredes en los bordes
                mapa[y][x] = new Pared(x, y);
            } else if (x < ancho - 1 && y < alto - 1 && x < ancho - 2) {
                mapa[y][x] = new Punto(x, y);
            }
        }
    }

    // Generar power-ups en posiciones aleatorias
    srand(time(nullptr));
    for (int i = 0; i < 6; i++) {
        int x = rand() % (ancho - 3) + 1;
        int y = rand() % (alto - 3) + 1;

        // verificar que no haya ya un objeto en esa posición
        if (mapa[y][x] != nullptr) {
            mapa[y][x]=nullptr;
            mapa[y][x+1]=nullptr;
        }
        mapa[y][x] = new PowerUp(x, y, "+10");
    }
}

void Mapa::setPuente(int y, int size) {
    if (y < 0 || y >= alto) return;

    for (int x = 1; x < ancho - 1; x++) {
        if (x < size || x >= ancho - size) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }
}

void Mapa::setXPuente(int x, int size) {
    if (x < 0 || x >= ancho) return;

    for (int y = 1; y < alto - 1; y++) {
        if (y < size || y >= alto - size) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }
}

void Mapa::setVerticalLine(int x, int startY, int endY) {
    if (x < 0 || x >= ancho) return;

    int ymin = std::max(1, startY);
    int ymax = std::min(alto - 2, endY);

    for (int y = ymin; y <= ymax; y++) {
        if (mapa[y][x] != nullptr) {
            delete mapa[y][x];
        }
        // crear nueva pared
        mapa[y][x] = new Pared(x, y);
        mapa[y][x-1] = nullptr;
    }
}

void Mapa::setHorizontalLine(int y, int startX, int endX) {
    if (y < 0 || y >= alto) return;

    int xmin = std::max(1, startX);
    int xmax = std::min(ancho - 2, endX);

    for (int x = xmin; x <= xmax; x++) {
        // si ya existe un objeto, liberarlo primero
        if (mapa[y][x] != nullptr) {
            delete mapa[y][x];
        }
        mapa[y][x] = new Pared(x, y);
    }
}

Object* Mapa::getObjectAt(int x, int y) const { //CAMBIOS
    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
        return nullptr;
    }
    return mapa[y][x];
}

bool Mapa::isWall(int x, int y) const { //CAMBIOS
    Object* obj = getObjectAt(x, y);
    return obj && dynamic_cast<Pared*>(obj) != nullptr;
}

void Mapa::clearArea(int x, int y) {
    if (x < 0 || x >= ancho || y < 0 || y >= alto) return;

    if (mapa[y][x] != nullptr) {
        delete mapa[y][x];
        mapa[y][x] = nullptr;
    }
}

// Getters
int Mapa::getScore() const {
    return score;
}

int Mapa::getVidas() const {
    return vidas;
}

int Mapa::getAncho() const {
    return ancho;
}

int Mapa::getAlto() const {
    return alto;
}

// Setters
void Mapa::setScore(int newScore) {
    score = newScore;
}

void Mapa::setVidas(int newVidas) {
    vidas = newVidas;
}

// Métodos de juego
void Mapa::addScore(int puntos) {
    score += puntos;
}

void Mapa::loseLife() {
    if (vidas > 0) {
        vidas--;
    }
}

std::pair<int,int> Mapa::findFreeCellNear(int cx, int cy) const { //CAMBIOS
    int rango = 2; // busca celdas libres en un cuadrado alrededor del centro
    for (int dy = -rango; dy <= rango; dy++) {
        for (int dx = -rango; dx <= rango; dx++) {
            int nx = cx + dx;
            int ny = cy + dy;
            if (!isWall(nx, ny) && mapa[ny][nx] == nullptr) {
                return {nx, ny};
            }
        }
    }
    return {cx, cy}; // fallback
}

void Mapa::draw() {
    // dibujar todos los objetos de la matriz
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                mapa[y][x]->draw();
            }
        }
    }

    for (PowerUp* power : powerups) {
        if (power) {
            power->draw();
        }
    }

    // informacion
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    mvprintw(maxY - 3, 0, "Score: %d", score);
    mvprintw(maxY - 2, 0, "Vidas: %d", vidas);

    size_t objectCount = 0;
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                objectCount++;
            }
        }
    }

    mvprintw(maxY - 1, 0, "Dimensiones: %dx%d | Objetos: %zu", ancho, alto, objectCount);
    mvprintw(maxY - 3, maxX - 12, "Q para salir");
}