#include "globalHeader.h"
#include "utilities.h"

//plot 1 pixel given location and colour
void xy_plot_pixel(int x, int y, short int line_color);
void point_plot_pixel(point* location, short int line_color);

//clear screen
void clear_screen();

//helper for draw line
void swap(int *a, int *b);

//draws a line using Bresenham's algorithm
void draw_line(int x0, int y0, int x1, int y1, short int colour);

//draw player square
void draw_player_square(Square* square);
	

