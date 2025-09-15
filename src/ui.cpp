/* Descripción del archivo: Lógica para dibujar las pantallas declaradas en ui.h con ncurses */

#include "./../headers/ui.h" // Inclusión de las declaraciones de las funciones
#include <vector> // Para std::vector en el menú
#include <string> // Para std::string

// DECLARACIÓN DE FUNCIONES AUXILIARES ESENCIALES

// Centralización del texto en la pantalla
void print_centered(int starty, const std::string& text) {
    int startx = (COLS - text.length()) / 2;
    mvprintw(starty, startx, "%s", text.c_str());
}

// Configuración e inicialización de la terminal para usar ncurses
void setupNcurses() {
    initscr();            // Inicialización de ncurses
    noecho();             // No mostrar las teclas presionadas
    cbreak();             // Procesamiento de entrada inmediato
    curs_set(0);          // Oculta el cursor
    keypad(stdscr, TRUE); // Habilitando teclas especiales (flechas)
    start_color();        // Habilitando colores
    // Definición de pares de colores (ID, color de texto, color de fondo)
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Para Pac-Man y texto
    init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Para texto normal
}

// Restauranción de la terminal (estado normal) al cerrar programa
void closeNcurses() {
    endwin();
}

/* Tarea 1: 
   - Implementación de menú principal y navegación mediante:
        * Teclas WASD
        * Flechas 
        * Enter para seleccionar */
int drawMainMenu() {
    std::vector<std::string> options = {"Iniciar Juego", "Salir"};
    int choice = 0;
    int key;

    while (true) {
        clear(); // Limpiar pantalla

        // Dibujo del título del juego
        attron(COLOR_PAIR(1) | A_BOLD);
        print_centered(5, "P A C - M A N 🤓");
        print_centered(6, "===============");
        attroff(COLOR_PAIR(1) | A_BOLD);

        // Dibujo de las opciones
        for (size_t i = 0; i < options.size(); ++i) {
            if (i == choice) {
                attron(A_REVERSE); // Resaltar la opción seleccionada
            }
            print_centered(10 + i, options[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }
        
        print_centered(15, "Usa las flechas para navegar y Enter para seleccionar");

        refresh();

        key = getch(); // Esperando entrada del usuario

        switch (key) {
            case KEY_UP:
            case 'w': // Uso de flecha arriba o 'w'
                choice = (choice - 1 + options.size()) % options.size();
                break;
            case KEY_DOWN:
            case 's': // Uso de flecha abajo o 's'
                choice = (choice + 1) % options.size();
                break;
            case 10: // Enter = 10 en código ASCII
                return choice; // Devuelve 0 o 1
        }
    }
}