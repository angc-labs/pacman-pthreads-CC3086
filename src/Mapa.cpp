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

Mapa::Mapa() : score(0), vidas(4) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    ancho = maxX;
    alto = maxY - 3;

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

    for (PowerUp* power : powerups) {
        delete power;
    }
    powerups.clear();
}

void Mapa::generarMapa(int modalidad) {
    switch (modalidad) {
        case 0:
            generarMapaClasico();
            break;
        case 1:
            generarMapaRapido();
            break;
        case 2:
            generarMapaDosJugadores();
            break;
        default:
            generarMapaClasico();
            break;
    }
}

// MAPA 1: MODO CLÁSICO - Tu diseño original
void Mapa::generarMapaClasico() {
    // Limpiar mapa previo
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }

    for (PowerUp* power : powerups) {
        delete power;
    }
    powerups.clear();

    // Paredes del borde y puntos en el resto
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (y == 0 || y == alto - 1 || x == 0 || x == ancho - 1) {
                mapa[y][x] = new Pared(x, y);
            } else {
                mapa[y][x] = new Punto(x, y);
            }
        }
    }

    // Crear la caja de los fantasmas en el centro
    int cx = ancho / 2;
    int cy = alto / 2;
    for (int y = cy - 2; y <= cy + 2; y++) {
        for (int x = cx - 5; x <= cx + 5; x++) {
            if (y == cy - 2 || y == cy + 2 || x == cx - 5 || x == cx + 5) {
                mapa[y][x] = new Pared(x, y);
            } else {
                clearArea(x, y);
            }
        }
    }

    // Puerta más ancha conectada al pasillo central
    clearArea(cx - 1, cy - 2);
    clearArea(cx, cy - 2);
    clearArea(cx + 1, cy - 2);
    clearArea(cx - 1, cy - 3);
    clearArea(cx, cy - 3);
    clearArea(cx + 1, cy - 3);

    // Línea horizontal central PARTIDA con apertura más ancha
    int yCentral = cy;
    setHorizontalLine(yCentral, 2, cx - 7);
    setHorizontalLine(yCentral, cx + 7, ancho - 3);
    clearArea(cx - 1, yCentral);
    clearArea(cx, yCentral);
    clearArea(cx + 1, yCentral);

    // Muros verticales y horizontales segmentados
    setVerticalLine(ancho / 4, 2, cy - 4);
    setVerticalLine(ancho / 4, cy + 4, alto - 3);
    setVerticalLine(3 * ancho / 4, 2, cy - 4);
    setVerticalLine(3 * ancho / 4, cy + 4, alto - 3);

    setHorizontalLine(alto / 4, 2, cx - 6);
    setHorizontalLine(alto / 4, cx + 6, ancho - 3);
    setHorizontalLine(3 * alto / 4, 2, cx - 6);
    setHorizontalLine(3 * alto / 4, cx + 6, ancho - 3);

    // Pasillos centrales hacia afuera
    clearArea(cx, 1);
    clearArea(cx, alto - 2);
    clearArea(1, cy);
    clearArea(ancho - 2, cy);

    // Power-ups en las esquinas
    mapa[1][1] = new PowerUp(1, 1, "+10");
    mapa[1][ancho - 2] = new PowerUp(ancho - 2, 1, "+10");
    mapa[alto - 2][1] = new PowerUp(1, alto - 2, "+10");
    mapa[alto - 2][ancho - 2] = new PowerUp(ancho - 2, alto - 2, "+10");
}

// MAPA 2: MODO RÁPIDO - Variación del clásico con más espacios abiertos
void Mapa::generarMapaRapido() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }

    for (PowerUp* power : powerups) {
        delete power;
    }
    powerups.clear();

    // Paredes del borde y puntos
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (y == 0 || y == alto - 1 || x == 0 || x == ancho - 1) {
                mapa[y][x] = new Pared(x, y);
            } else {
                mapa[y][x] = new Punto(x, y);
            }
        }
    }

    int cx = ancho / 2;
    int cy = alto / 2;

    // Caja central (mismo tamaño que el clásico)
    for (int y = cy - 2; y <= cy + 2; y++) {
        for (int x = cx - 5; x <= cx + 5; x++) {
            if (y == cy - 2 || y == cy + 2 || x == cx - 5 || x == cx + 5) {
                mapa[y][x] = new Pared(x, y);
            } else {
                clearArea(x, y);
            }
        }
    }
    // Puerta más ancha arriba
    clearArea(cx - 1, cy - 2);
    clearArea(cx, cy - 2);
    clearArea(cx + 1, cy - 2);
    clearArea(cx - 1, cy - 3);
    clearArea(cx, cy - 3);
    clearArea(cx + 1, cy - 3);

    // Muros horizontales más cortos (más espacios para maniobrar)
    setHorizontalLine(alto / 3, 2, cx - 8);
    setHorizontalLine(alto / 3, cx + 8, ancho - 3);
    setHorizontalLine(2 * alto / 3, 2, cx - 8);
    setHorizontalLine(2 * alto / 3, cx + 8, ancho - 3);

    // Muros verticales más espaciados
    setVerticalLine(ancho / 5, 2, cy - 5);
    setVerticalLine(ancho / 5, cy + 5, alto - 3);
    setVerticalLine(4 * ancho / 5, 2, cy - 5);
    setVerticalLine(4 * ancho / 5, cy + 5, alto - 3);

    // Muros adicionales para complejidad moderada
    setHorizontalLine(cy - 8, ancho / 6, cx - 3);
    setHorizontalLine(cy - 8, cx + 3, ancho - ancho / 6);
    setHorizontalLine(cy + 8, ancho / 6, cx - 3);
    setHorizontalLine(cy + 8, cx + 3, ancho - ancho / 6);

    setVerticalLine(cx / 2, alto / 5, cy - 3);
    setVerticalLine(cx / 2, cy + 3, 4 * alto / 5);
    setVerticalLine(cx + cx / 2, alto / 5, cy - 3);
    setVerticalLine(cx + cx / 2, cy + 3, 4 * alto / 5);

    // Pasillos de escape
    clearArea(cx, 1);
    clearArea(cx, alto - 2);
    clearArea(1, cy);
    clearArea(ancho - 2, cy);

    // Más power-ups
    mapa[1][1] = new PowerUp(1, 1, "+10");
    mapa[1][ancho - 2] = new PowerUp(ancho - 2, 1, "+10");
    mapa[alto - 2][1] = new PowerUp(1, alto - 2, "+10");
    mapa[alto - 2][ancho - 2] = new PowerUp(ancho - 2, alto - 2, "+10");
    mapa[cy][ancho / 6] = new PowerUp(ancho / 6, cy, "+10");
    mapa[cy][ancho - ancho / 6] = new PowerUp(ancho - ancho / 6, cy, "+10");
}

// MAPA 3: DOS JUGADORES - Diseño simétrico basado en el clásico
void Mapa::generarMapaDosJugadores() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (mapa[y][x] != nullptr) {
                delete mapa[y][x];
                mapa[y][x] = nullptr;
            }
        }
    }

    for (PowerUp* power : powerups) {
        delete power;
    }
    powerups.clear();

    // Paredes del borde y puntos
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (y == 0 || y == alto - 1 || x == 0 || x == ancho - 1) {
                mapa[y][x] = new Pared(x, y);
            } else {
                mapa[y][x] = new Punto(x, y);
            }
        }
    }

    int cx = ancho / 2;
    int cy = alto / 2;

    // Caja central (mismo tamaño que el clásico)
    for (int y = cy - 2; y <= cy + 2; y++) {
        for (int x = cx - 5; x <= cx + 5; x++) {
            if (y == cy - 2 || y == cy + 2 || x == cx - 5 || x == cx + 5) {
                mapa[y][x] = new Pared(x, y);
            } else {
                clearArea(x, y);
            }
        }
    }
    // Puerta más ancha arriba (por donde salen los fantasmas)
    clearArea(cx - 1, cy - 2);
    clearArea(cx, cy - 2);
    clearArea(cx + 1, cy - 2);
    clearArea(cx - 1, cy - 3);
    clearArea(cx, cy - 3);
    clearArea(cx + 1, cy - 3);
    // Puerta más ancha abajo (entrada alternativa)
    clearArea(cx - 1, cy + 2);
    clearArea(cx, cy + 2);
    clearArea(cx + 1, cy + 2);
    clearArea(cx - 1, cy + 3);
    clearArea(cx, cy + 3);
    clearArea(cx + 1, cy + 3);

    // Muro vertical central divisorio (con aperturas)
    setVerticalLine(cx, 2, cy - 8);
    setVerticalLine(cx, cy + 8, alto - 3);

    // LADO IZQUIERDO
    // Muros horizontales
    setHorizontalLine(alto / 4, 2, cx / 2 - 2);
    setHorizontalLine(alto / 2 - 5, 2, cx / 2 + 2);
    setHorizontalLine(alto / 2 + 5, 2, cx / 2 + 2);
    setHorizontalLine(3 * alto / 4, 2, cx / 2 - 2);

    // Muros verticales
    setVerticalLine(cx / 4, 2, alto / 4);
    setVerticalLine(cx / 4, 3 * alto / 4, alto - 3);
    setVerticalLine(cx / 2 - 4, cy / 2, cy - 4);
    setVerticalLine(cx / 2 - 4, cy + 4, alto - cy / 2);
    setVerticalLine(3 * cx / 4, alto / 5, cy - 3);
    setVerticalLine(3 * cx / 4, cy + 3, 4 * alto / 5);

    // LADO DERECHO (simétrico)
    // Muros horizontales
    setHorizontalLine(alto / 4, cx + cx / 2 + 2, ancho - 3);
    setHorizontalLine(alto / 2 - 5, cx + cx / 2 - 2, ancho - 3);
    setHorizontalLine(alto / 2 + 5, cx + cx / 2 - 2, ancho - 3);
    setHorizontalLine(3 * alto / 4, cx + cx / 2 + 2, ancho - 3);

    // Muros verticales
    setVerticalLine(cx + 3 * cx / 4, 2, alto / 4);
    setVerticalLine(cx + 3 * cx / 4, 3 * alto / 4, alto - 3);
    setVerticalLine(cx + cx / 2 + 4, cy / 2, cy - 4);
    setVerticalLine(cx + cx / 2 + 4, cy + 4, alto - cy / 2);
    setVerticalLine(cx + cx / 4, alto / 5, cy - 3);
    setVerticalLine(cx + cx / 4, cy + 3, 4 * alto / 5);

    // Pasillos de salida
    clearArea(cx, 1);
    clearArea(cx, alto - 2);
    clearArea(1, cy);
    clearArea(ancho - 2, cy);

    // Power-ups simétricos
    mapa[1][1] = new PowerUp(1, 1, "+10");
    mapa[1][ancho - 2] = new PowerUp(ancho - 2, 1, "+10");
    mapa[alto - 2][1] = new PowerUp(1, alto - 2, "+10");
    mapa[alto - 2][ancho - 2] = new PowerUp(ancho - 2, alto - 2, "+10");
    mapa[cy][cx / 3] = new PowerUp(cx / 3, cy, "+10");
    mapa[cy][ancho - cx / 3] = new PowerUp(ancho - cx / 3, cy, "+10");
}

// Métodos auxiliares (sin cambios)
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
        mapa[y][x] = new Pared(x, y);
    }
}

void Mapa::setHorizontalLine(int y, int startX, int endX) {
    if (y < 0 || y >= alto) return;
    int xmin = std::max(1, startX);
    int xmax = std::min(ancho - 2, endX);
    for (int x = xmin; x <= xmax; x++) {
        if (mapa[y][x] != nullptr) {
            delete mapa[y][x];
        }
        mapa[y][x] = new Pared(x, y);
    }
}

Object* Mapa::getObjectAt(int x, int y) const {
    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
        return nullptr;
    }
    return mapa[y][x];
}

bool Mapa::isWall(int x, int y) const {
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

int Mapa::getScore() const { return score; }
int Mapa::getVidas() const { return vidas; }
int Mapa::getAncho() const { return ancho; }
int Mapa::getAlto() const { return alto; }
void Mapa::setScore(int newScore) { score = newScore; }
void Mapa::setVidas(int newVidas) { vidas = newVidas; }
void Mapa::addScore(int puntos) { score += puntos; }
void Mapa::loseLife() { if (vidas > 0) vidas--; }

std::pair<int,int> Mapa::findFreeCellNear(int cx, int cy) const {
    int rango = 2;
    for (int dy = -rango; dy <= rango; dy++) {
        for (int dx = -rango; dx <= rango; dx++) {
            int nx = cx + dx;
            int ny = cy + dy;
            if (!isWall(nx, ny) && mapa[ny][nx] == nullptr) {
                return {nx, ny};
            }
        }
    }
    return {cx, cy};
}

void Mapa::draw() {
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