#include "collisions.h"

// get number of pixels in a line from Bresenham's algo. Needed for numOfPixels and line data members.
int getNumOfPixelsInLine(point *start, point *end){
    int numOfPixels = 0;
    int x0 = start->x;
    int y0 = start->y;
    int x1 = end->x;
    int y1 = end->y;
    bool is_steep = abs(y1 - y0) > abs(x1 - x0);

    if (is_steep)
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = -(deltax / 2);
    int y_step = 0;
    int y = y0;
    if (y0 < y1)
    {
        y_step = 1;
    }
    else
    {
        y_step = -1;
    }

    for (int x = x0; x <= x1; x++)
    {
        if (is_steep)
        {
            // xy_plot_pixel(y, x, colour);
            numOfPixels++;
        }
        else
        {
            // xy_plot_pixel(x, y, colour);
            numOfPixels++;
        }
        error = error + deltay;
        if (error > 0)
        {
            y = y + y_step;
            error = error - deltax;
        }
    }
    return numOfPixels;
}
// get array of points corresponding to each pixel in the line using Bresenham's algo
point **getPixelsInLine(point *start, point *end, int size){
    int x0 = start->x;
    int y0 = start->y;
    int x1 = end->x;
    int y1 = end->y;
    point **pixels = (point **)malloc(size * sizeof(point *));
    if (pixels == NULL){
        printf("failed");
        return NULL;
    }
        bool is_steep = abs(y1 - y0) > abs(x1 - x0);

    if (is_steep)
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = -(deltax / 2);
    int y_step = 0;
    int y = y0;
    if (y0 < y1)
    {
        y_step = 1;
    }
    else
    {
        y_step = -1;
    }
    int i = 0;
    for (int x = x0; x <= x1; x++)
    {
        if (is_steep)
        {
            // xy_plot_pixel(y, x, colour);
            pixels[i] = pointStruct(y, x);
        }
        else
        {
            // xy_plot_pixel(x, y, colour);
            pixels[i] = pointStruct(x, y);
        }
        i++;
        error = error + deltay;
        if (error > 0)
        {
            y = y + y_step;
            error = error - deltax;
        }
    }
    return pixels;
}

//'constructor' for barriers
barrier *barrierStruct(point *start, point *end, point *direction){

    int numOfPixels = getNumOfPixelsInLine(start, end);
    barrier *newBarrier = malloc(sizeof(barrier) + numOfPixels * sizeof(point*));

    newBarrier->start = start;
    newBarrier->end = end;
    newBarrier->direction = direction;
    newBarrier->numOfPixels = numOfPixels;
    newBarrier -> line = getPixelsInLine(start, end, numOfPixels);

    return newBarrier;
}
//'destructor' for barriers
void freeBarrier(barrier *barrier){
    free(barrier->start);
    free(barrier->end);
    free(barrier->direction);
    for (int i = 0; i < barrier->numOfPixels; i++)
    {
        free(barrier->line[i]);
    }
    free(barrier);
}