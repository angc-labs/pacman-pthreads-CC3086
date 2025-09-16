/* Alejandra Saraí Avilés González - 24722
 * Descripción del archivo: Lógica para dibujar las pantallas declaradas en ui.h con ncurses
 * Fecha de última modificación: 16 / 09 / 2025 */

#include "ui.h" // Para incluir las declaraciones de las funciones
#include <vector> // Para std::vector en el menú
#include <string> // Para std::string
#include <ncurses.h>
#include "highscore.h" // Para incluir las declaraciones de las funciones

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
    std::vector<std::string> options = {"Iniciar Juego", "Ver Puntajes", "Salir"};
    int choice = 0;
    int key;

    while (true) {
        clear(); // Limpiar pantalla

        // Dibujo del título del juego
        attron(COLOR_PAIR(1) | A_BOLD);
        print_centered(5, "P A C - M A N");
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

/* Tarea 2: 
   - Mostrar puntación:
        * Se dibuja el Head-Up Display con la información del juego. */
void drawGameHUD(int score) {
    attron(COLOR_PAIR(2) | A_BOLD);
    // Ajuste al ancho de la terminal mediante COLS
    mvprintw(1, 2, "Score: %d", score);
    attroff(COLOR_PAIR(2) | A_BOLD);
}

/* Tarea adicional:
    - Mostrar contenido de la pantalla de puntajes. */
void display_highscore_screen() {
    std::vector<Score> scores = load_scores();
    int scores_to_show = 10;
    int total_scores = scores.size();

    char input;
    while (true) {
        clear(); // Limpiando la pantalla

        // Dibujar el título
        mvprintw(3, 10, "      PUNTAJES DESTACADOS       ");
        mvprintw(4, 10, "================================");

    if (scores.empty()) {
        mvprintw(7, 12, "Aun no hay puntajes. !Se el primero!"); // No hay puntajes en el archivo
    } else {
        mvprintw(6, 12, "POS   NOMBRE      PUNTAJE"); // Encabezados a mostrar
        int y_pos = 8;
        int rank = 1;

        for (const auto& s : scores) {
            if (rank > scores_to_show) { // Salida del bucle al terminar de mostrar lo deseado
                break;
            }
            mvprintw(y_pos, 12, "%-5d %-10s  %d", rank, s.name.c_str(), s.score);
            y_pos++;
            rank++;
            if (rank > 10) break; // Devolución de los 10 primeros lugares
            }
        }

        int bottom_line = getmaxy(stdscr) - 3;
        
        if (scores_to_show < total_scores) {
            mvprintw(bottom_line, 10, "Presiona 'm' para ver mas puntajes.");
        }
        
        mvprintw(bottom_line + 1, 10, "Presiona 'q' para volver al menu principal.");
        refresh(); // Actualización de la pantalla para mostrar lo dibujado
        input = getch(); // Esperando a que el usuario presione una tecla

        if (input == 'q') break;
        
        if (input == 'm' && scores_to_show < total_scores) {
            scores_to_show += 10;
        }
    }   
}

/* Tarea adicional 2:
    - Implementación de la pantalla final de juego */
void handle_end_of_game(int final_score) {
    clear();
    mvprintw(11, 10, "         JUEGO TERMINADO         ");
    mvprintw(12, 10, "=================================");
    mvprintw(14, 10, "Tu puntaje final es: %d", final_score);
    mvprintw(16, 10, "Ingresa tu nombre (3 caracteres): ");
    refresh();

    char player_name[4];
    echo(); 
    wgetnstr(stdscr, player_name, 3);
    noecho(); 

    save_score(std::string(player_name), final_score);

    mvprintw(18, 10, "Puntaje guardado. Presiona una tecla para continuar...");
    refresh();
    getch();
}