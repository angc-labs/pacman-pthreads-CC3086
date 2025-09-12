/*
 * Angel Gabriel Chavez Otzoy
 * 24248
 * 12 / 09 / 2025
 */
#include <ncurses.h>

class Object {
protected:
    int x, y;
    char sprite;
    
public:
    // contructor
    Object(int x = 0, int y = 0, char sprite = ' ') : x(x), y(y), sprite(sprite) {}
    virtual ~Object() {}

    // dibujar objeto en algun lugar de la pantalla
    virtual void draw() {
        mvaddch(y, x, sprite);
    }

    // control de posicion
    virtual void update() {}

    // getters
    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
    // setters
    void setPos(int newX, int newY) {
        x = newX;
        y = newY;
    }
};