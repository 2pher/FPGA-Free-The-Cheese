#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// Represents a pixel location on screen
typedef struct image {
  int x;
  int y;
  short int color;

} image;

// Point (x,y)
typedef struct point {
  int x;
  int y;

} point;

//'Constructor' function for point struct
point* pointStruct(int x, int y);
void freePoint(point* p);

// Add two points together
point* addPoints(point* point1, point* point2);

// Hitbox for square
typedef struct squareHitbox {
  point* centre;
  point* topLeftPoint;
  point* bottomRightPoint;

} squareHitbox;

//'Constructor' functions for rectangles
squareHitbox* centreSquareHitboxStruct(point* centre, int sideLength);
squareHitbox* cornerPointsSquareHitboxStruct(point* topLeftPoint,
                                             point* bottomRightPoint);

// Hitbox for circles. Intended to be moving obstacles
typedef struct circleHitbox {
  point centre;
  int radius;

} circleHitbox;

//'Constructor for circle hitbox
circleHitbox* circleHitboxStruct(point* centre, int radius);

// 3 versions of distance formula using xy or point structs
double xyxyDistanceFormula(int x1, int y1, int x2, int y2);
double xyPointDistanceFormula(int x1, int y1, point* point2);
double pointPointDistanceFormula(point* point1, point* point2);

#endif
