/* Descripción del archivo: Lógica para dibujar las pantallas declaradas en ui.h con ncurses */

#include "ui.h" // Inclusión de las declaraciones de las funciones
#include <vector> // Para std::vector en el menú
#include <string> // Para std::string
#include <ncurses.h>
#include "highscore.h" // Para incluir las declaraciones de las funciones
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <list>

// DECLARACIÓN DE FUNCIONES AUXILIARES ESENCIALES

// Música en el juego
pid_t music_pid = -1;
std::list<pid_t> sound_pids; // Lista para almacenar los PIDs de los sonidos

void start_music() { // Iniciar música de fondo
    if (music_pid == -1) {
        music_pid = fork();
        if (music_pid == 0) {
            //Ejecutar el comando de música
            execlp("sh", "sh", "-c", "while true; do mpg123 ./resources/music.mp3; done", (char *)NULL); //FALTA AGREGAR LA MÚSICA EN ESPECÍFICO
            _exit(1); // Si execlp falla
        }
    }
}

void stop_music() { // Detener música de fondo
    if (music_pid != -1) {
        kill(music_pid, SIGTERM);
        waitpid(music_pid, NULL, 0);
        music_pid = -1;
    }
    // Detener todos los sonidos activos
    for (pid_t pid : sound_pids) {
        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
    }
    sound_pids.clear();
}

bool is_music_playing() { // Verificar si la música está sonando
    return music_pid != -1;
}

// Sonidos de efectos
void play_sound(const char* sound_file) { // Función genérica para reproducir un sonido
    pid_t sound_pid = fork();

    if (sound_pid == 0) {
        execlp("mpg123", "mpg123", "-q", sound_file, (char *)NULL);
        _exit(1);
    } else if (sound_pid > 0) {
        sound_pids.push_back(sound_pid); // Guarda el PID
    }
}

void play_start_sound() { // Sonido de inicio
    play_sound("./resources/start.mp3"); 
}

void play_move_sound() { // Sonido de movimiento de fantasmas
    play_sound("./resources/move.mp3"); 
}

void play_eat_sound() { // Sonido de comer punto
    play_sound("./resources/eat.mp3");
}

void play_die_sound() { // Sonido de morir
    play_sound("./resources/die.mp3");
}

void play_powerup_sound() { // Sonido de power-up
    play_sound("./resources/powerup.mp3"); 
}

void play_gameover_sound() { // Sonido de game over
    play_sound("./resources/gameover.mp3");
}

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

// Restauración de la terminal (estado normal) al cerrar programa
void closeNcurses() {
    endwin();
}

/* Tarea 1: 
   - Implementación de menú principal y navegación mediante:
        * Teclas WASD
        * Flechas 
        * Enter para seleccionar
        * Tecla 'm' para alternar música de fondo 
*/
int drawMainMenu() {
    std::vector<std::string> options = {"Iniciar Juego","Instrucciones","Ver puntaje","Salir"};
    int choice = 0;
    int key;

    while (true) {
        clear(); // Limpiar pantalla

        // Dibujo del título del juego
        attron(COLOR_PAIR(1) | A_BOLD);
        print_centered(5,"P A C - M A N ");
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
            case 'm': // Tecla 'm' para alternar música
                if (is_music_playing()) { // Si la música está sonando, detenerla
                    stop_music();
                }else { // Si no está sonando, iniciarla
                    start_music();
                }
                break;
        }
    }
}

/*
* Dibujar la pantalla de instrucciones - Version simplificada
*/
void drawInstructions() {
    clear();
    
    // Encabezado de instrucciones
    attron(COLOR_PAIR(1) | A_BOLD);
    print_centered(3, "INSTRUCCIONES DEL JUEGO");
    print_centered(4, "=======================");
    attroff(COLOR_PAIR(1) | A_BOLD);

    // Instrucciones del juego
    std::vector<std::string> instructions = {
        "OBJETIVO:",
        " - Comer todos los puntos del laberinto",
        " - Evitar a los fantasmas que te persiguen",
        " - Usar power-ups para ventajas temporales",
        "",
        "CONTROLES:",
        " - Flechas: Mover a Pac-Man (arriba, abajo, izquierda, derecha)",
        " - P: Pausar/Reanudar juego",
        " - Q: Salir al menu principal",
        " - M: Alternar música de fondo"
        "",
        "ELEMENTOS:",
        " - C: Pac-Man (jugador)",
        " - G: Fantasmas (enemigos)",
        " - .: Puntos (10 puntos cada uno)",
        " - P: Power-ups (efectos especiales)",
        " - #: Paredes (obstaculos)",
        "",
        "VIDAS:",
        " - Comienzas con 3 vidas",
        " - Pierdes 1 vida si un fantasma te atrapa",
        "",
        "Presiona cualquier tecla para volver al menu principal"
    };

    // Dibujar instrucciones
    for (size_t i = 0; i < instructions.size(); ++i) {
        if (instructions[i] == "OBJETIVO:" || 
            instructions[i] == "CONTROLES:" || 
            instructions[i] == "ELEMENTOS:" || 
            instructions[i] == "VIDAS:") {
            attron(COLOR_PAIR(1) | A_BOLD);
        }
        
        print_centered(6 + i, instructions[i]);
        
        if (instructions[i] == "OBJETIVO:" || 
            instructions[i] == "CONTROLES:" || 
            instructions[i] == "ELEMENTOS:" || 
            instructions[i] == "VIDAS:") {
            attroff(COLOR_PAIR(1) | A_BOLD);
        }
    }

    refresh();
    getch(); // Esperar a que el usuario presione una tecla
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
            mvprintw(bottom_line, 10, "Presiona 'r' para ver mas puntajes.");
        }
        
        mvprintw(bottom_line + 1, 10, "Presiona 'q' para volver al menu principal.");
        refresh(); // Actualización de la pantalla para mostrar lo dibujado
        input = getch(); // Esperando a que el usuario presione una tecla

        if (input == 'q' || input == 'Q') break;
        
        if (input == 'r' && scores_to_show < total_scores) {
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
    refresh();

    // validando nombre
    std::string playerName = getPlayerName(); 

    // Guardando el puntaje junto al nombre validado
    save_score(playerName, final_score);

    mvprintw(18, 10, "Puntaje guardado. Presiona una tecla para continuar...");
    refresh();
    getch();
}

std::string getPlayerName() {
    std::string playerName; // Guarda el nombre del jugador

    while (true) { // Bucle se romperá con una entrada válida
        char input_buffer[21]; 

        // Limpiando mensajes de error previos
        move(18, 10); // Mueve el cursor a la línea de error
        clrtoeol();   // Borra la línea desde el cursor hasta el final

        // Muestra la instrucción para el usuario
        mvprintw(16, 10, "Ingresa tu nombre (3-20 caracteres): ");
        refresh();

        // Activa el eco para que el usuario vea lo que escribe
        echo(); 
        
        // Captura la entrada del usuario.
        // Limitando automáticamente los caracteres a 20.
        wgetnstr(stdscr, input_buffer, 20);

        // Desactivando el eco
        noecho();

        // Conversión del buffer de C a un string de C++ para facilitar el manejo
        playerName = std::string(input_buffer);

        // Validación de la extensión del nombre del jugador
        if (playerName.length() < 3) { // En caso el nombre sea demasiado corto
            mvprintw(18, 10, "ERROR: El nombre debe tener al menos 3 caracteres.");
            refresh();
            napms(1500); // Pausa de 1.5 segundos para que el usuario lea el error
        } else { // Entrada entre 3 y 20 caracteres
            break; 
        }
    }
    
    // Limpieza del mensaje de error/instrucción final antes de salir
    move(16, 10); clrtoeol();
    move(18, 10); clrtoeol();

    return playerName;
}