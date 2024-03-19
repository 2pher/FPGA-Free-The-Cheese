#include "globals.h"
#include "utilities.h"

//'constructor' function for point structs
point* pointStruct(int x1, int y1){
    point* newPoint;
    newPoint -> x = x1;
    newPoint -> y = y1;
    return newPoint;
}

//'constructor' function for square hitbox structs
//only for odd valued sideLength
squareHitbox* centreSquareHitboxStruct(point* centre, int sideLength){
    squareHitbox* hitbox;
    hitbox -> centre = centre;
    point* topLeft = pointStruct(centre -> x - (sideLength - 1) / 2, centre -> y - (sideLength - 1) / 2);
    point* bottomRight = pointStruct(centre -> x + (sideLength - 1) / 2, centre -> y + (sideLength - 1) / 2);
    hitbox -> topLeftPoint = topLeft;
    hitbox -> bottomRightPoint = bottomRight;
    return hitbox;
}

//given corners, may not necessarily be a square
squareHitbox* cornerPointsSquareHitboxStruct(point* topLeft, point* bottomRight){
    squareHitbox* hitbox;
    hitbox -> centre = pointStruct(-1, -1);
    hitbox -> topLeftPoint = topLeft;
    hitbox -> bottomRightPoint = bottomRight;
    return hitbox;
}

//circleHitbox 'constructor'
circleHitbox* circleHitboxStruct(point* centre, int radius){
//to be completed



}

//3 versions of distance formula, implemented via normal distance formula eqn. It's in double but could 
//be rounded to int later if neccessary
double xyxyDistanceFormula(int x1, int y1, int x2, int y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double xyPointDistanceFormula(int x1, int y1, point* point2){
    return sqrt(pow(point2 -> x - x1, 2) + pow(point2 -> y - y1, 2));
}

double pointPointDistanceFormula(point* point1, point* point2){
    return sqrt(pow(point2 -> x - point1 -> x, 2) + pow(point2 -> y - point1 -> y, 2));
}