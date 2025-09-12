/*
* Universidad del Valle de Guatemala
* PROGRAMACIÓN DE MICROPROCESADORES
* Sección: 20
* Autora: Tiffany Salazar Suarez
* Carnét: 24630
*/

#include <iostream>
#include <ncurses.h>
#include <vector>
#include "Object.h"
#include "PowerUp.h"

using namespace std;

class Mapa(){
    private:
        Vector<Object*> mapa; //Vector para almacenar punteros a objetos (paredes, puntos, etc.)
        int score, vidas, ancho, alto; //Puntuación, vidas, ancho y alto del mapa
        Vector<PowerUp*> powerups; //Vector para almacenar punteros a power-ups

    public:
        Mapa(int ancho, int alto) : ancho(ancho), alto(alto), score(0), vidas(3) {} //Constructor

        ~Mapa() { //Destructor
            for (auto obj : mapa) delete obj;
            for (auto p : powerups) delete p;
        }

        void generarMapa(){ //Función para generar el mapa
            mapa.clear();
            powerups.clear();

            for (int y = 0; y < alto; y++){
                for (int x = 0; x < ancho; x++){
                    if (y == 0 || y == alto - 1 || x == 0 || x == ancho - 1){
                        mapa.push_back(new Pared(x, y));   //Derivada de Object
                    }else{
                        mapa.push_back(new Punto(x, y));   //Derivada de Object
                    }
                }
            }
        }

        void setYPuente(y, size){ //Función para crear el puente vertical
            for (int x = 1; x < ancho - 1; x++){
                if (x < size || x >= ancho - size){
                    Object* obj = getObjectAt(x, y);
                    if (obj) obj->setSprite(' '); //Borra la pared/punto
                }
            }
        }

        void setXPuente(x, size){ //Función para crear el puente horizontal
            for (int y = 1; y < alto - 1; y++){
                if (y < size || y >= alto - size){
                    Object* obj = getObjectAt(x, y);
                    if (obj) obj->setSprite(' '); //Borra la pared/punto
                }
            }
        }

        void setVerticalLine(x, startY, endY){ //Función para establecer una línea vertical (pared)
            ymin = max(1, startY);
            ymax = min(alto - 2, endY);
            for (int y = startY; y <= endY; y++){
                Object* obj = getObjectAt(x, y);
                if (obj) obj->setSprite('#'); //Establece la pared
            }
        }

        Object* getObjectAt(x, y){ //Función para obtener un objeto en una posición específica
            for (Object* obj : mapa){
                if (obj -> getX() == x && obj -> getY() == y){
                    return obj;
                }
            }
            return nullptr;
        }

        bool isWall(x, y);{ //Función para verificar si hay una pared en una posición específica
            Object* obj = getObjectAt(x, y);
            return obj && obj->getSprite() == '#';
        } 

        //Agregados 
        void draw(){ //Función para dibujar el mapa
            for (Object* obj : mapa){
                obj->draw();
            }
            mvprintw(alto, 0, "Score: %d   Vidas: %d", score, vidas);
        }

        void addScore(int puntos){ //Función para aumentar la puntuación 
            score += puntos; 
        }

        void loseLife(){ //Función para perder una vida 
            vidas--; 
        } 

        int getScore() const{ //Función para obtener la puntuación 
            return score;
        }

        int getVidas() const{ //Función para obtener las vidas
            return vidas;
        }

}
