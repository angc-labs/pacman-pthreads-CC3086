#include <iostream>
#include <ncurses.h>

int main() {
    // Inicializar ncurses
    initscr();
    printw("Hello, ncurses!\n");
    refresh();
    getch();
    endwin();
    return 0;
}
