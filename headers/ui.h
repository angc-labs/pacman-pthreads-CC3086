#ifndef UI_H
#define UI_H

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <string>
#include <vector>

// Dibuja el menú principal y devuelve la opción seleccionada por el usuario.
// Opciones: 0 para "Iniciar Juego", 1 para "Salir".
int drawMainMenu();

// Función auxiliar para inicializar la configuración de ncurses.
void setupNcurses();

// Función para dibujar la pantalla de instrucciones.
void drawInstructions();

// Función auxiliar para cerrar ncurses.
void closeNcurses();

// Dibuja el contenido de la pantalla de puntajes
void display_highscore_screen();

// Maneja la pantalla de fin de juego y guardado de puntaje
void handle_end_of_game(int final_score);

// Para validación de la extensión de caracteres del nombre
std::string getPlayerName(); 

#endif // UI_H