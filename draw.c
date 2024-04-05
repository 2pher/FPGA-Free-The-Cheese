#include "draw.h"

extern volatile int pixel_buffer_start;
extern bool level1, level2;

// plot 1 pixel given xy and colour
void xy_plot_pixel(int x, int y, short int line_color) {
  volatile short int *one_pixel_address;
  one_pixel_address = (pixel_buffer_start + (y << 10) + (x << 1));
  *one_pixel_address = line_color;
}
// plot 1 pixel given location and colour
void point_plot_pixel(point *location, short int line_color) {
  volatile short int *one_pixel_address;
  one_pixel_address =
      (pixel_buffer_start + (location->y << 10) + (location->x << 1));
  *one_pixel_address = line_color;
}

void drawTitleScreen(void) {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, TITLE_SCREEN[y][x]);
    }
  }
}

/* void drawBackground(void) {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, BACKGROUND[y][x]);
    }
  }
} */

void drawMouse(int mouse, bool draw) {
  if (mouse == 1) {
    for (int i = 0; i < (sizeof(MOUSE1) / sizeof(MOUSE1[0])); i++) {
      if (draw)
        xy_plot_pixel(MOUSE1[i].x, MOUSE1[i].y, MOUSE1[i].color);
      else
        xy_plot_pixel(MOUSE1[i].x, MOUSE1[i].y,
                      TITLE_SCREEN[MOUSE1[i].y][MOUSE1[i].x]);
    }
  } else if (mouse == 2) {
    for (int i = 0; i < (sizeof(MOUSE2) / sizeof(MOUSE2[0])); i++) {
      if (draw)
        xy_plot_pixel(MOUSE2[i].x, MOUSE2[i].y, MOUSE2[i].color);
      else
        xy_plot_pixel(MOUSE2[i].x, MOUSE2[i].y,
                      TITLE_SCREEN[MOUSE2[i].y][MOUSE2[i].x]);
    }
  } else {
    for (int i = 0; i < (sizeof(MOUSE3) / sizeof(MOUSE3[0])); i++) {
      if (draw)
        xy_plot_pixel(MOUSE3[i].x, MOUSE3[i].y, MOUSE3[i].color);
      else
        xy_plot_pixel(MOUSE3[i].x, MOUSE3[i].y,
                      TITLE_SCREEN[MOUSE3[i].y][MOUSE3[i].x]);
    }
  }
}

void drawLevel1() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, LEVEL1[y][x]);
    }
  }
}

void drawLevel2() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, LEVEL2[y][x]);
    }
  }
}

// clear screen
void clear_screen() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, 0x0);
    }
  }
}

// helper for draw line
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// draws a line using Bresenham's algorithm
void draw_line(int x0, int y0, int x1, int y1, short int colour) {
  bool is_steep = abs(y1 - y0) > abs(x1 - x0);

  if (is_steep) {
    swap(&x0, &y0);
    swap(&x1, &y1);
  }
  if (x0 > x1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }

  int deltax = x1 - x0;
  int deltay = abs(y1 - y0);
  int error = -(deltax / 2);
  int y_step = 0;
  int y = y0;
  if (y0 < y1) {
    y_step = 1;
  } else {
    y_step = -1;
  }

  for (int x = x0; x <= x1; x++) {
    if (is_steep) {
      xy_plot_pixel(y, x, colour);
    } else {
      xy_plot_pixel(x, y, colour);
    }
    error = error + deltay;
    if (error > 0) {
      y = y + y_step;
      error = error - deltax;
    }
  }
}

// draw player square, ONLY ODD VALUED SQUARESIZE
void draw_player_square(Square *square) {
  int half_side_length = (square->sideLength - 1) / 2;
  for (int x = square->position->x - half_side_length;
       x <= square->position->x + half_side_length; x++) {
    for (int y = square->position->y - half_side_length;
         y <= square->position->y + half_side_length; y++) {
      xy_plot_pixel(x, y, 0x01FF);
    }
  }
}

// erase player square
void erase_player_square(point *oldSquare, Square *newSquare, int level) {
  if (oldSquare->x != newSquare->position->x ||
      oldSquare->y != newSquare->position->y) {
    Square *square = squareStruct(oldSquare, 9);
    int half_side_length = (square->sideLength - 1) / 2;
    for (int x = square->position->x - half_side_length;
         x <= square->position->x + half_side_length; x++) {
      for (int y = square->position->y - half_side_length;
           y <= square->position->y + half_side_length; y++) {
        if (level == 1) xy_plot_pixel(x, y, LEVEL1[y][x]);
        if (level == 2) xy_plot_pixel(x, y, LEVEL2[y][x]);
      }
    }
  }
}

// Draw all circles
void drawCircles(Circle *circle[], point *oldCircle[], int size, int level) {
  for (int i = 0; i < size; i++) {
    erase_circle(oldCircle[i], level);
    draw_circle(circle[i]);
  }
}

// draw circle obstacle
void draw_circle(Circle *circle) {
  int half_side_length = 2;
  if (level2) half_side_length = 10;
  for (int x = circle->position->x - half_side_length;
       x <= circle->position->x + half_side_length; x++) {
    for (int y = circle->position->y - half_side_length;
         y <= circle->position->y + half_side_length; y++) {
      xy_plot_pixel(x, y, 0xF800);
    }
  }

/*   int top = ceil(circle->position->y - circle->radius);
  int bottom = floor(circle->position->y + circle->radius);

  for (int y = top; y <= bottom; y++) {
    int dy = y - circle->position->y;
    float dx = sqrt(circle->radius * circle->radius - dy * dy);
    int left = ceil(circle->position->x - dx);
    int right = floor(circle->position->x + dx);
    for (int x = left; x <= right; x++) {
      xy_plot_pixel(x, y, 0xF800);
    }
  } */
}

void erase_circle(point *circle, int level) {
  int half_side_length = 2;
  if (level2) half_side_length = 10;
  for (int x = circle->x - half_side_length;
       x <= circle->x + half_side_length; x++) {
    for (int y = circle->y - half_side_length;
         y <= circle->y + half_side_length; y++) {
      if (level == 1) xy_plot_pixel(x, y, LEVEL1[y][x]);
      if (level == 2) xy_plot_pixel(x, y, LEVEL2[y][x]);
    }
  }
}

//draw cheese
void draw_cheese(Cheese* cheese){
  int halfSideLength = cheese -> halfSideLength;
  for (int i = 0; i < sizeof(CHEESE)/sizeof(CHEESE[0]); i++) {
    xy_plot_pixel(CHEESE[i].x + cheese->position->x - halfSideLength, CHEESE[i].y + cheese->position->y - halfSideLength, CHEESE[i].color);
  }
}

void checkForCheese(Square* square, Cheese* cheese[], int size){
  for(int i = 0; i < size; i++){
    if (!cheese[i]->collected) {
      int square_x1 = square->position->x - ((square->sideLength - 1) / 2);
      int square_x2 = square->position->x + ((square->sideLength - 1) / 2);
      int square_y1 = square->position->y - ((square->sideLength - 1) / 2);
      int square_y2 = square->position->y + ((square->sideLength - 1) / 2);

      int cheese_x1 = cheese[i]->position->x - cheese[i]->halfSideLength;  // Left
      int cheese_x2 = cheese[i]->position->x + cheese[i]->halfSideLength;  // Right
      int cheese_y1 = cheese[i]->position->y - cheese[i]->halfSideLength;  // Top
      int cheese_y2 = cheese[i]->position->y + cheese[i]->halfSideLength;  // Bottom

      // Check if squares have collided from any direction
      if (square_x1 <= cheese_x2 && square_x2 >= cheese_x1 &&
          square_y1 <= cheese_y2 && square_y2 >= cheese_y1) {
        cheese[i]->collected = true;  // Collision detected
        if (level2) square->respawn = cheese[i]->position;
        erase_cheese(cheese[i]);
      }
    } else if (cheese[i]->collected && !cheese[i]->erasedTwice) {
      erase_cheese(cheese[i]);
      cheese[i]->erasedTwice = true;
    }
  }
}

//erase cheese
void erase_cheese(Cheese* cheese){
   int halfSideLength = cheese -> halfSideLength;
  for(int x = cheese -> position -> x - halfSideLength; x <= cheese -> position -> x + halfSideLength; x++){
    for(int y = cheese -> position -> y - halfSideLength; y <= cheese -> position -> y + halfSideLength; y++){
      if(level1) xy_plot_pixel(x, y, LEVEL1[y][x]);
      if(level2) xy_plot_pixel(x, y, LEVEL2[y][x]);
    }
  }
}

// Wait for screen to finish drawing
void wait_for_vsync() {
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
  int bufferStatusBit;
  *(pixel_ctrl_ptr) =
      1;  // Write 1 into buffer register, causing a frame buffer swap.
  bufferStatusBit = *(pixel_ctrl_ptr + 3) & 1;  // isolates S bit
  // implementing v-sync (waiting for S bit to go low)
  while (bufferStatusBit != 0) {
    bufferStatusBit = *(pixel_ctrl_ptr + 3) & 1;
  }
}
