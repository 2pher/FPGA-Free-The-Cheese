#ifndef COLLISIONS_H_
#define COLLISIONS_H_

#include "utilities.h"

//get number of pixels in a line from Bresenham's algo. Needed for numOfPixels and line data members. 
int getNumOfPixelsInLine(point* start, point* end);

//get array of points corresponding to each pixel in the line using Bresenham's algo
point** getPixelsInLine(point* start, point* end, int size);

//barrier object. A line segment that the player square will collide against.
typedef struct barrier
{

    point* start;
    point* end;
    point* direction;
    int numOfPixels;
    point** line;

} barrier;

//'constructor' for barriers
barrier* barrierStruct(point* start, point* end, point* direction);

//'destructor' for barriers
void freeBarrier(barrier* barrier);

#endif