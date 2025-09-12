/* Descripción del archivo: Definición de las funciones para implementar el juego */

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

#endif // UI_H