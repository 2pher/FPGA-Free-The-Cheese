#ifndef DRAW_H
#define DRAW_H

#include "globalHeader.h"
#include "levels.h"
#include "shapes.h"
#include "death_counter.h"
#include "title_screen.h"
#include "utilities.h"

// plot 1 pixel given location and colour
void xy_plot_pixel(int x, int y, short int line_color);
void point_plot_pixel(point* location, short int line_color);

void drawTitleScreen();
// void drawBackground();
void drawMouse(int mouse, bool draw);

void drawLevel1();
void drawLevel2();

// clear screen
void clear_screen();

// helper for draw line
void swap(int* a, int* b);

// draws a line using Bresenham's algorithm
void draw_line(int x0, int y0, int x1, int y1, short int colour);

// draw player square
void draw_player_square(Square* square);
void erase_player_square(point* oldSquare, Square* square, int level);

// draw circle obstacle
void drawCircles(Circle* circle[], point* oldCircle[], int size, int level);
void draw_circle(Circle* circle);
void erase_circle(point* circle, int level);
void checkForCheese(Square* square, Cheese* cheese[], int size);

//draw cheese
void draw_cheese(Cheese* cheese);
//erase cheese
void erase_cheese(Cheese* cheese);

// Vsync helper function
void wait_for_vsync();

void drawLevelCount(int count);
void drawCheeseCounter(int max_count);
void updateCheeseCounter();

void drawTimer();
void updateTimer();
void drawColons();


#endif
