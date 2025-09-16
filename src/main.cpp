#include <iostream>
#include <ncurses.h>
#include <unistd.h> // Necesario para la función usleep()
#include "highscore.h" // Para usar la funcion display_highscore_screen 
#include "ui.h" // Para usar la funcion drawMainMenu

int gameLoop() {
    // Inicializar ncurses
    printw("PROYECTO 1 - MICROPROCESADORES");

    int score = 0;
    int ch; // Almacena la tecla presionada

    nodelay(stdscr, TRUE); // Modo no bloqueante (para input)

    while (true) {
        ch = getch(); // Intenta obtener una tecla. Devuelve ERR si no hay

        // Si se presiona 'q', sale del bucle del juego
        if (ch == 'q' || ch == 'Q'){
            break;
        }

        clear();

        // Llamada a la función para mostrar puntuación
        drawGameHUD(score);
    
        refresh(); // Actualizando la pantalla para mostrar cambios

        // Pausa de 0.1 segundo para que el juego no corra muy rápido
        usleep(100000);
    }

    nodelay(stdscr, FALSE); // Regresa a modo bloqueante

    return score; // Devolución de puntaje final
}

int main() {
    setupNcurses(); // Función auxiliar para configurar ncurses correctamente

    // Bucle principal que mantiene el programa en el menú
    while (true) {
        int menuChoice = drawMainMenu(); // Muestra el menú y espera elección de usuario

        if (menuChoice == 0) { // Opción de "Iniciar Juego"
            int final_score = gameLoop(); // Lanza bucle de juego y espera devolución de puntaje
            handle_end_of_game(final_score); // Llamada a función para guardar puntaje
        } else if (menuChoice == 1) { // Opción de "Ver puntajes"
            display_highscore_screen();
        } else if (menuChoice == 2) { // Opción de "Salir"
            break; // Termina el programa
        }
    }

    closeNcurses(); // Restauración de configuración original de la terminal

    refresh();
    getch();
    endwin();
    return 0;
}