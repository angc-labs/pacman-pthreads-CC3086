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

// Función auxiliar para cerrar ncurses.
void closeNcurses();

#endif // UI_H