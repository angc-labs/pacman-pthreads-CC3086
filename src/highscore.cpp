/* Descripción del archivo: Implementación de las funciones destinadas al manejo 
                            de los nombres de los jugadores y sus respectivos puntajes */

// Importando los recursos necesarios
#include "highscore.h"
#include <fstream>
#include <algorithm>
#include <iostream>

// Declaración de las funciones necesarias para la pantalla de puntajes //

// Comparando dos puntajes (ordenamiento)
bool compareScores(const Score& a, const Score& b) {
    return a.score > b.score;
}

std::vector<Score> load_scores() {
    std::vector<Score> scores;
    std::ifstream file("puntajes.txt"); // Carga del archivo
    if (!file.is_open()) {
        return scores; // Devolución del vector vacío si no hay archivo
    }

    Score temp;
    while (file >> temp.name >> temp.score) {
        scores.push_back(temp);
    }
    file.close();

    // Ordenamiento de puntajes antes de devolverlos
    std::sort(scores.begin(), scores.end(), compareScores);
    return scores;
}

// Guardar los puntajes
void save_score(const std::string& name, int score) {
    std::vector<Score> scores = load_scores();
    scores.push_back({name, score});
    // Ordenando la lista con el nuevo puntaje
    std::sort(scores.begin(), scores.end(), compareScores);

    std::ofstream file("puntajes.txt");
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de puntajes para escribir." << std::endl;
        return;
    }
    
    // Guardando los puntajes en el archivo
    for (const auto& s : scores) {
        file << s.name << " " << s.score << std::endl;
    }
    file.close();
}