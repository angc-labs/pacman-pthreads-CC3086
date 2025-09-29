#ifndef UI_H
#define UI_H

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

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

// Dibuja el menú de selección de modalidad de juego y devuelve la opción seleccionada
int drawGameModeMenu();

// Declaración de la función gameLoop con valor por defecto que es el modo clásico
int gameLoop(int gameMode = 0);

// Para validación de la extensión de caracteres del nombre
std::string getPlayerName(); 

// Funciones para manejar la música de fondo
void start_music();
void stop_music();
bool is_music_playing();
void play_start_sound();
void play_move_sound();
void play_eat_sound();
void play_die_sound();
void play_powerup_sound();
void play_gameover_sound();

#endif // UI_H