#include "utilities.h"

/*******************************************************************************
 * Image constructor
 ******************************************************************************/
image* imageStruct(int x1, int y1, short int color1) {
  image* newImage = malloc(sizeof(image));
  if (newImage != NULL) {
    newImage->x = x1;
    newImage->y = y1;
    newImage->color = color1;
  }
  return newImage;
}

void freeImage(image* i) { free(i); }

/*******************************************************************************
 * Point constructor
 ******************************************************************************/
point* pointStruct(int x1, int y1) {
  point* newPoint = malloc(sizeof(point));  // Allocate memory
  if (newPoint != NULL) {  // Check if memory allocation was successful
    newPoint->x = x1;
    newPoint->y = y1;
  }
  return newPoint;
}

void freePoint(point* p) { free(p); }

/*******************************************************************************
 * Helper function to add 2 points
 ******************************************************************************/
point* addPoints(point* point1, point* point2) {
  point* newPoint = pointStruct(point1->x + point2->x, point1->y + point2->y);
  return newPoint;
}

/*******************************************************************************
 * Hitbox constructor
 ******************************************************************************/
squareHitbox* centreSquareHitboxStruct(point* centre, int sideLength) {
  squareHitbox* hitbox = malloc(sizeof(squareHitbox));  // Allocate memory
  if (hitbox != NULL) {  // Check if memory allocation was successful
    hitbox->centre = centre;
    point* topLeft = pointStruct(centre->x - (sideLength - 1) / 2,
                                 centre->y - (sideLength - 1) / 2);
    point* bottomRight = pointStruct(centre->x + (sideLength - 1) / 2,
                                     centre->y + (sideLength - 1) / 2);
    hitbox->topLeftPoint = topLeft;
    hitbox->bottomRightPoint = bottomRight;
  }
  return hitbox;
}

// given corners, may not necessarily be a square
squareHitbox* cornerPointsSquareHitboxStruct(point* topLeft,
                                             point* bottomRight) {
  squareHitbox* hitbox;
  hitbox->centre = pointStruct(-1, -1);
  hitbox->topLeftPoint = topLeft;
  hitbox->bottomRightPoint = bottomRight;
  return hitbox;
}

/*******************************************************************************
 * Circle hitbox constructor
 ******************************************************************************/
circleHitbox* circleHitboxStruct(point* centre, int radius) {
  // to be completed
}

/*******************************************************************************
 * Distance formulas
 ******************************************************************************/
double xyxyDistanceFormula(int x1, int y1, int x2, int y2) {
  // return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  return 0;
}

double xyPointDistanceFormula(int x1, int y1, point* point2) {
  // return sqrt(pow(point2 -> x - x1, 2) + pow(point2 -> y - y1, 2));
  return 0;
}

double pointPointDistanceFormula(point* point1, point* point2) {
  // return sqrt(pow(point2 -> x - point1 -> x, 2) + pow(point2 -> y - point1 ->
  // y, 2));
  return 0;
}