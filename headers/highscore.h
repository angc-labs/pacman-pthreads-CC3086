/* Descripción del archivo: Definición de las estructuras de las funciones para el 
                            manejo de los nombres de los jugadores y sus respectivos puntajes */

// Importando recursos necesarios

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>

// Declaración de la estructura para los puntajes
struct Score {
    std::string name;
    int score;
};

// Carga de los puntajes desde el archivo "puntajes.txt"
std::vector<Score> load_scores();

// Guardado de un nuevo puntaje en el archivo "puntajes.txt"
void save_score(const std::string& name, int score);

#endif // HIGHSCORE_H