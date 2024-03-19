#include "globals.h"
#include "utilities.h"

//player square struct
typedef struct Square {

    point* position;
    point* velocity;
    point* acceleration;
    int sideLength;
    squareHitbox* hitbox;
    int hitboxSideLength;
    bool isAlive;

} Square;

//square 'constructor'
Square* squareStruct(point* position, int sideLength);

//move square based on key press
void moveSquare(Square* square, bool upKey,  bool downKey, bool rightKey, bool leftKey);

//update square properties on gameloop
void updateSquare(Square* square);

//circle struct
typedef struct Circle {

    point* position;
    point* velocity;
    point* acceleration;
    int radius;
    circleHitbox* hitbox;
    int hitboxRadius;
    point** path;
    bool isVisible;

} Circle;

//circle 'constructor'
Circle* circleStruct(point* position, int radius, point** path);

