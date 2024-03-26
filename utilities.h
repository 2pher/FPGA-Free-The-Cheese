#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// represents a pixel location on screen
typedef struct point {
  int x;
  int y;

} point;

//'constructor' function for point struct
point* pointStruct(int x, int y);
void freePoint(point* p);

// add two points together
point* addPoints(point* point1, point* point2);

// hitbox for square
typedef struct squareHitbox {
  point* centre;
  point* topLeftPoint;
  point* bottomRightPoint;

} squareHitbox;

//'constructor' functions for rectangles
squareHitbox* centreSquareHitboxStruct(point* centre, int sideLength);
squareHitbox* cornerPointsSquareHitboxStruct(point* topLeftPoint,
                                             point* bottomRightPoint);

// hitbox for circles. Intended to be moving obstacles
typedef struct circleHitbox {
  point centre;
  int radius;

} circleHitbox;

//'constructor for circle hitbox
circleHitbox* circleHitboxStruct(point* centre, int radius);

// 3 versions of distance formula using xy or point structs
double xyxyDistanceFormula(int x1, int y1, int x2, int y2);

double xyPointDistanceFormula(int x1, int y1, point* point2);

double pointPointDistanceFormula(point* point1, point* point2);

#endif
