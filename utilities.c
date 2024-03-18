#include "globals.h"
#include "utilities.h"

//'constructor' function for point structs
point* pointStruct(int x1, int y1){
    point* newPoint;
    newPoint -> x = x1;
    newPoint -> y = y1;
    return newPoint;
}

//'constructor' function for rectangle hitbox structs
rectangleHitbox* rectangleHitboxStruct(point centre, int length, int width){
    
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