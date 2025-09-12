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
#include "../headers/Object.h"
#include "../headers/PowerUp.h"
#include "../headers/Pared.h"
#include "../headers/Punto.h"

using namespace std;

class Mapa{
    private:
        vector<Object*> mapa; //Vector para almacenar punteros a objetos (paredes, puntos, etc.)
        int score, vidas, ancho, alto; //Puntuación, vidas, ancho y alto del mapa
        vector<PowerUp*> powerups; //Vector para almacenar punteros a power-ups

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


        void setYPuente(int y, int size){ //Función para crear el puente vertical
            for (int x = 1; x < ancho - 1; x++){
                if (x < size || x >= ancho - size){
                    Object* obj = getObjectAt(x, y);
                    if (obj) obj->setSprite(' '); //Borra la pared/punto
                }
            }
        }

        void setXPuente(int x, int size){ //Función para crear el puente horizontal
            for (int y = 1; y < alto - 1; y++){
                if (y < size || y >= alto - size){
                    Object* obj = getObjectAt(x, y);
                    if (obj) obj->setSprite(' '); //Borra la pared/punto
                }
            }
        }

        void setVerticalLine(int x, int startY, int endY){ //Función para establecer una línea vertical (pared)
            int ymin = std::max(1, startY);
            int ymax = std::min(alto - 2, endY);
            for (int y = ymin; y <= ymax; y++){
                Object* obj = getObjectAt(x, y);
                if (obj) obj->setSprite('#'); //Establece la pared
            }
        }

        Object* getObjectAt(int x, int y){ //Función para obtener un objeto en una posición específica
            for (Object* obj : mapa){
                if (obj->getX() == x && obj->getY() == y){
                    return obj;
                }
            }
            return nullptr;
        }

        bool isWall(int x, int y){ //Función para verificar si hay una pared en una posición específica
            Object* obj = getObjectAt(x, y);
            // Si quieres usar el sprite, necesitas un getter
            // return obj && obj->getSprite() == '#';
            // Como no existe getSprite, puedes comparar indirectamente:
            // Si el objeto es de tipo Pared, entonces es pared
            return obj && dynamic_cast<Pared*>(obj) != nullptr;
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
};
