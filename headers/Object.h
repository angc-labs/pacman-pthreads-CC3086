//
//  9/12/25.
//

#ifndef OBJECT_H
#define OBJECT_H

class Object {
protected:
    int x, y;
    char sprite;

public:
    // contructor
    Object(int x = 0, int y = 0, char sprite = ' ');
    virtual ~Object();

    // objeto en pantalla
    virtual void draw();

    // control de posicion
    virtual void update();

    // getters
    int getX() const;
    int getY() const;

    // setters
    void setPos(int newX, int newY);
};
#endif //OBJECT_H
