/* Alejandra Saraí Avilés González - 24722
 * Descripción del archivo: Definición de las estructuras de las funciones para implementar el juego 
 * Fecha de última modificación: 16 / 09 / 2025 */

#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <string>
#include <vector>

// Dibuja el menú principal y devuelve la opción seleccionada por el usuario.
// Opciones: 0 para "Iniciar Juego", 1 para "Salir".
int drawMainMenu();

// Dibuja el Head-Up Display (HUD) con la información del juego.
void drawGameHUD(int score);

// Función auxiliar para inicializar la configuración de ncurses.
void setupNcurses();

// Función auxiliar para cerrar ncurses.
void closeNcurses();

// Dibuja el contenido de la pantalla de puntajes
void display_highscore_screen();

// Maneja la pantalla de fin de juego y guardado de puntaje
void handle_end_of_game(int final_score);

#endif // UI_H