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
  point* respawn;
  bool isAlive;

} Square;

// square 'constructor'
Square* squareStruct(point* position, int sideLength);
void freeSquare(Square* s);

// move square based on key press
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
  squareHitbox* hitbox;
  int hitboxRadius;
  bool isVisible;

} Circle;

// circle 'constructor'
Circle* circleStruct(point* position, int radius, point* velocity);
void freeCircle(Circle* circle);

void moveCircles(Circle* circle[], int size);
void moveCircles2(Circle* circle[], int size);
void moveCircles3(Circle* circle[], int size);

// cheese struct
typedef struct Cheese {
  point* position;
  bool collected;
  bool erasedTwice;
  int halfSideLength;

} Cheese;

// coin constructor
Cheese* cheeseStruct(point* position);
// destructor
void freeCheese(Cheese* cheese);

// Check for collisions
void checkForCollisions(Square* square, Circle* circle[], int size);
bool collided(Square* square, Circle* circle);
bool checkBoundaryDiagonal(Square* square, int dx, int dy);

#endif