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

Mapa::Mapa() : score(0), vidas(3) {
    // Obtener dimensiones automáticamente
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    ancho = maxX;
    alto = maxY - 3;  // Reservar 3 líneas para información
}

Mapa::~Mapa() {
    // Liberar memoria de todos los objetos
    for (Object* obj : mapa) {
        delete obj;
    }
    for (PowerUp* power : powerups) {
        delete power;
    }
}

void Mapa::generarMapa() {
    // Limpiar vectores existentes
    for (Object* obj : mapa) {
        delete obj;
    }
    for (PowerUp* power : powerups) {
        delete power;
    }
    mapa.clear();
    powerups.clear();

    // Generar solo las paredes del borde y algunos puntos
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (y == 0 || y == alto - 1 || x == 0 || x == ancho - 1) {
                // Paredes en los bordes
                mapa.push_back(new Pared(x, y));
            } else if (x < ancho - 1 && y < alto-1) {
                // Algunos puntos esparcidos (no llenar toda la pantalla)
                mapa.push_back(new Punto(x, y));
            }
        }
    }
    for (int i=0; i < 6; i++) {
        int x = rand() % (ancho - 2) + 1;
        int y = rand() % (alto - 2) + 1;
        mapa.push_back(new PowerUp(x, y, "+10"));
    }

}

void Mapa::setPuente(int y, int size) {
    for (int x = 1; x < ancho - 1; x++) {
        if (x < size || x >= ancho - size) {
            Object* obj = getObjectAt(x, y);
            if (obj) {
                // Remover el objeto del vector y liberar memoria
                auto it = std::find(mapa.begin(), mapa.end(), obj);
                if (it != mapa.end()) {
                    delete *it;
                    mapa.erase(it);
                }
            }
        }
    }
}

void Mapa::setXPuente(int x, int size) {
    for (int y = 1; y < alto - 1; y++) {
        if (y < size || y >= alto - size) {
            Object* obj = getObjectAt(x, y);
            if (obj) {
                // Remover el objeto del vector y liberar memoria
                auto it = std::find(mapa.begin(), mapa.end(), obj);
                if (it != mapa.end()) {
                    delete *it;
                    mapa.erase(it);
                }
            }
        }
    }
}

void Mapa::setVerticalLine(int x, int startY, int endY) {
    int ymin = std::max(1, startY);
    int ymax = std::min(alto - 2, endY);

    for (int y = ymin; y <= ymax; y++) {
        // Verificar si ya existe un objeto en esa posición
        Object* existing = getObjectAt(x, y);
        if (!existing) {
            // Si no existe, crear una nueva pared
            mapa.push_back(new Pared(x, y));
        } else {
            // Si existe y no es pared, reemplazarlo
            Pared* wall = dynamic_cast<Pared*>(existing);
            if (!wall) {
                // Remover el objeto existente y poner una pared
                auto it = std::find(mapa.begin(), mapa.end(), existing);
                if (it != mapa.end()) {
                    delete *it;
                    mapa.erase(it);
                }
                mapa.push_back(new Pared(x, y));
            }
        }
    }
}

Object* Mapa::getObjectAt(int x, int y) {
    for (Object* obj : mapa) {
        if (obj && obj->getX() == x && obj->getY() == y) {
            return obj;
        }
    }
    return nullptr;
}

bool Mapa::isWall(int x, int y) {
    Object* obj = getObjectAt(x, y);
    return obj && dynamic_cast<Pared*>(obj) != nullptr;
}

void Mapa::clearArea(int x, int y) {
    Object* obj = getObjectAt(x, y);
    if (obj) {
        auto it = std::find(mapa.begin(), mapa.end(), obj);
        if (it != mapa.end()) {
            delete *it;
            mapa.erase(it);
        }
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

void Mapa::draw() {
    // Dibujar todos los objetos del mapa
    for (Object* obj : mapa) {
        if (obj) {
            obj->draw();
        }
    }

    // Dibujar power-ups
    for (PowerUp* power : powerups) {
        if (power) {
            power->draw();
        }
    }

    // Mostrar información en la parte inferior
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    mvprintw(maxY - 3, 0, "Score: %d", score);
    mvprintw(maxY - 2, 0, "Vidas: %d", vidas);
    mvprintw(maxY - 1, 0, "Dimensiones: %dx%d | Objetos: %zu",
             ancho, alto, mapa.size());
}