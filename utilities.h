#include "globals.h"

//represents a pixel location on screen
typedef struct point {

    int x;
    int y;

} point;

//'constructor' function for point struct
point* pointStruct(int x, int y);

//rectangle hitboxes. Can use for 
typedef struct squareHitbox {

    point* centre;
    point* topLeftPoint;
    point* bottomRightPoint;

} squareHitbox;

//'constructor' functions for rectangles
squareHitbox* centreSquareHitboxStruct(point* centre, int sideLength);
squareHitbox* cornerPointsSquareHitboxStruct(point* topLeftPoint, point* bottomRightPoint);

typedef struct circleHitbox {
    
    point centre;
    int radius;

} circleHitbox;

circleHitbox* circleHitboxStruct(point* centre, int radius);

//3 versions of distance formula using xy or point structs
double xyxyDistanceFormula(int x1, int y1, int x2, int y2);

double xyPointDistanceFormula(int x1, int y1, point* point2);

double pointPointDistanceFormula(point* point1, point* point2);





