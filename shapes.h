#ifndef SHAPES_H
#define SHAPES_H

#include "globalHeader.h"
#include "levels.h"
#include "utilities.h"

// player square struct
typedef struct Square {
  point* position;
  point* velocity;
  point* acceleration;
  int sideLength;
  squareHitbox* hitbox;
  int hitboxSideLength;
  bool isAlive;

} Square;

// square 'constructor'
Square* squareStruct(point* position, int sideLength);
void freeSquare(Square* s);

// move square based on key press
void moveSquare(Square* square);
// no acceleration, only linear movement speed
void moveSquareNoAcc(Square* square);

bool checkBoundaryLeft(Square* square);
bool checkBoundaryRight(Square* square);
bool checkBoundaryUp(Square* square);
bool checkBoundaryDown(Square* square);

// update square properties on gameloop
void updateSquare(Square* square);

// circle struct
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

// circle 'constructor'
Circle* circleStruct(point* position, int radius, point** path);

#endif