#include <iostream>
#include <ncurses.h>
#include "ui.h" // Para usar las funciones drawMainMenu
#include <unistd.h> // Necesario para la función usleep()

void gameLoop() {
    // Inicializar ncurses
    initscr();
    printw("PROYECTO 1 - MICROPROCESADORES");

    int score = 0;
    int ch; // Almacena la tecla presionada

    nodelay(stdscr, TRUE);

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

    nodelay(stdscr, FALSE);
}

int main() {
    setupNcurses(); // Función auxiliar para configurar ncurses correctamente

    // Bucle principal que mantiene el programa en el menú
    while (true) {
        int menuChoice = drawMainMenu(); // Muestra el menú y espera elección de usuario

        if (menuChoice == 0) { // Opción de "Iniciar Juego"
            gameLoop(); // Lanzamiento del bucle del juego
        } else { // Opción de "Salir"
            break; // Termina el programa
        }
    }

    closeNcurses(); // Restauración de configuración original de la terminal

    refresh();
    getch();
    endwin();
    return 0;
}
