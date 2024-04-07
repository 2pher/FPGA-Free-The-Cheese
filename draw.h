#ifndef DRAW_H
#define DRAW_H

#include "death_counter.h"
#include "globalHeader.h"
#include "levels.h"
#include "shapes.h"
#include "title_screen.h"
#include "utilities.h"

// Plot 1 pixel given location and colour
void xy_plot_pixel(int x, int y, short int line_color);
void point_plot_pixel(point* location, short int line_color);

void drawTitleScreen();
void drawMouse(int mouse, bool draw);

// Draw each level
void drawLevel1();
void drawLevel2();
void drawLevel3();
void drawFinal();

// Clear screen
void clear_screen();

// Draw player square
void draw_player_square(Square* square);
void erase_player_square(point* oldSquare, Square* square, int level);

// Draw circle obstacle
void drawCircles(Circle* circle[], point* oldCircle[], int size, int level);
void draw_circle(Circle* circle);
void erase_circle(point* circle, int level);
void checkForCheese(Square* square, Cheese* cheese[], int size);

// Draw cheese
void draw_cheese(Cheese* cheese);
// Drase cheese
void erase_cheese(Cheese* cheese);

// Vsync helper function
void wait_for_vsync();

// Draw UI - level count, cheese count, timer
void drawLevelCount(int count);
void drawCheeseCounter(int max_count);
void updateCheeseCounter();
void drawTimer();
void updateTimer();
void drawColons();

#endif
