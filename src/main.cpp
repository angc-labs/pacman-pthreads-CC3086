#include <iostream>
#include <ncurses.h>

int main() {
    // Inicializar ncurses
    initscr();
    printw("PROYECTO 1 - MICROPROCESADORES");
    refresh();
    getch();
    endwin();
    return 0;
}
