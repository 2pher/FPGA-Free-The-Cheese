#include "globals.h"

//represents a pixel location on screen
typedef struct point {

    int x;
    int y;

} point;

point* pointStruct(int x, int y);

typedef struct rectangleHitbox {

    point centre;
    point bottomLeftPoint;
    point topRightPoint;

} rectangleHitbox;

rectangleHitbox* rectangleHitboxStruct(point centre, int length, int width);

typedef struct circleHitbox {
    
    point centre;
    int radius;

} circleHitbox;



//3 versions of distance formula using xy or point structs
double xyxyDistanceFormula(int x1, int y1, int x2, int y2);

double xyPointDistanceFormula(int x1, int y1, point* point2);

double pointPointDistanceFormula(point* point1, point* point2);





