#include "draw.h"

#include "audio_samples.h"
#include "interrupts.h"

/*******************************************************************************
 * Define external variables used / modifying
 ******************************************************************************/
extern volatile int pixel_buffer_start;
extern bool level1, level2, level3;
extern int CHEESE_COUNT;
extern audioDevice *audioBuffer;
extern int centiseconds, seconds, minutes;
extern int old_seconds, old_minutes;

/*******************************************************************************
 * Plot pixel, given x, y and line color
 ******************************************************************************/
void xy_plot_pixel(int x, int y, short int line_color) {
  volatile short int *one_pixel_address;
  one_pixel_address = (pixel_buffer_start + (y << 10) + (x << 1));
  *one_pixel_address = line_color;
}

/*******************************************************************************
 * Draw title screen
 ******************************************************************************/
void drawTitleScreen(void) {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, TITLE_SCREEN[y][x]);
    }
  }
}

/*******************************************************************************
 * Draw animating mouse depending on frame (draw)
 ******************************************************************************/
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

/*******************************************************************************
 * Print level 1 onto back buffer
 ******************************************************************************/
void drawLevel1() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, LEVEL1[y][x]);
    }
  }
}

/*******************************************************************************
 * Print level 2 onto back buffer
 ******************************************************************************/
void drawLevel2() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, LEVEL2[y][x]);
    }
  }
}

/*******************************************************************************
 * Print level 3 onto back buffer
 ******************************************************************************/
void drawLevel3() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, LEVEL3[y][x]);
    }
  }
}

/*******************************************************************************
 * Print end screen onto back buffer
 ******************************************************************************/
void drawFinal() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      if (x == 0 || x == 1) {
        xy_plot_pixel(x, y, 0x0000);
      } else {
        xy_plot_pixel(x, y, FINAL[y][x]);
      }
    }
  }
}

/*******************************************************************************
 * Clear screen
 ******************************************************************************/
void clear_screen() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, 0x0);
    }
  }
}

/*******************************************************************************
 * Draw player square based on its position and side length
 ******************************************************************************/
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

/*******************************************************************************
 * Erase player square by drawing background over previous position
 ******************************************************************************/
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
        if (level == 3) xy_plot_pixel(x, y, LEVEL3[y][x]);
      }
    }
  }
}

/*******************************************************************************
 * Loop to erase and draw all circles
 ******************************************************************************/
void drawCircles(Circle *circle[], point *oldCircle[], int size, int level) {
  for (int i = 0; i < size; i++) {
    erase_circle(oldCircle[i], level);
  }
  for (int i = 0; i < size; i++) {
    draw_circle(circle[i]);
  }
}

/*******************************************************************************
 * Draws circle based on position and radius
 ******************************************************************************/
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
}

/*******************************************************************************
 * Erases circle by printing level background on previous position
 ******************************************************************************/
void erase_circle(point *circle, int level) {
  int half_side_length = 2;
  if (level2) half_side_length = 10;
  for (int x = circle->x - half_side_length; x <= circle->x + half_side_length;
       x++) {
    for (int y = circle->y - half_side_length;
         y <= circle->y + half_side_length; y++) {
      if (level == 1) {
        xy_plot_pixel(x, y, LEVEL1[y][x]);
      } else if (level == 2) {
        xy_plot_pixel(x, y, LEVEL2[y][x]);
      } else {
        xy_plot_pixel(x, y, LEVEL3[y][x]);
      }
    }
  }
}

/*******************************************************************************
 * Draw image of cheese at desired position
 ******************************************************************************/
void draw_cheese(Cheese *cheese) {
  int halfSideLength = cheese->halfSideLength;
  for (int i = 0; i < sizeof(CHEESE) / sizeof(CHEESE[0]); i++) {
    xy_plot_pixel(CHEESE[i].x + cheese->position->x - halfSideLength,
                  CHEESE[i].y + cheese->position->y - halfSideLength,
                  CHEESE[i].color);
  }
}

/*******************************************************************************
 * Helper function to check if player collided with cheese:
 * - Update cheese count
 * - Play sound
 * - Update spawn point
 * - Update 'Cheese' struct to check if level is completed
 ******************************************************************************/
void checkForCheese(Square *square, Cheese *cheese[], int size) {
  for (int i = 0; i < size; i++) {
    if (!cheese[i]->collected) {
      int square_x1 = square->position->x - ((square->sideLength - 1) / 2);
      int square_x2 = square->position->x + ((square->sideLength - 1) / 2);
      int square_y1 = square->position->y - ((square->sideLength - 1) / 2);
      int square_y2 = square->position->y + ((square->sideLength - 1) / 2);

      int cheese_x1 =
          cheese[i]->position->x - cheese[i]->halfSideLength;  // Left
      int cheese_x2 =
          cheese[i]->position->x + cheese[i]->halfSideLength;  // Right
      int cheese_y1 =
          cheese[i]->position->y - cheese[i]->halfSideLength;  // Top
      int cheese_y2 =
          cheese[i]->position->y + cheese[i]->halfSideLength;  // Bottom

      // Check if squares have collided from any direction
      if (square_x1 <= cheese_x2 && square_x2 >= cheese_x1 &&
          square_y1 <= cheese_y2 && square_y2 >= cheese_y1) {
        cheese[i]->collected = true;  // Collision detected
        if (level2) square->respawn = cheese[i]->position;
        playAudio(EATCHEESE, CHEESE_SOUND);
        erase_cheese(cheese[i]);
        CHEESE_COUNT++;
        updateCheeseCounter();
      }
    } else if (cheese[i]->collected && !cheese[i]->erasedTwice) {
      erase_cheese(cheese[i]);
      updateCheeseCounter();
      cheese[i]->erasedTwice = true;
    }
  }
}

/*******************************************************************************
 * Erase a cheese if collided by printing level background over its position
 ******************************************************************************/
void erase_cheese(Cheese *cheese) {
  int halfSideLength = cheese->halfSideLength;
  for (int x = cheese->position->x - halfSideLength;
       x <= cheese->position->x + halfSideLength; x++) {
    for (int y = cheese->position->y - halfSideLength;
         y <= cheese->position->y + halfSideLength; y++) {
      if (level1) xy_plot_pixel(x, y, LEVEL1[y][x]);
      if (level2) xy_plot_pixel(x, y, LEVEL2[y][x]);
      if (level3) xy_plot_pixel(x, y, LEVEL3[y][x]);
    }
  }
}

/*******************************************************************************
 * V-sync helper function
 ******************************************************************************/
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

/*******************************************************************************
 * Draws level count - UI Element
 ******************************************************************************/
void drawLevelCount(int count) {
  for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
    xy_plot_pixel(NUM_3[i].x + 300, NUM_3[i].y + 7, 0xFFFF);
  }

  for (int i = 0; i < sizeof(SLASH) / sizeof(SLASH[0]); i++) {
    xy_plot_pixel(SLASH[i].x + 289, SLASH[i].y + 7, 0xFFFF);
  }

  if (count == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(NUM_1[i].x + 274, NUM_1[i].y + 7, 0xFFFF);
    }
  } else if (count == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(NUM_2[i].x + 274, NUM_2[i].y + 7, 0xFFFF);
    }
  } else if (count == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(NUM_3[i].x + 274, NUM_3[i].y + 7, 0xFFFF);
    }
  }
}

/*******************************************************************************
 * Draw cheese counter
 * - Print image of cheese
 * - 0 / (max_count)
 ******************************************************************************/
void drawCheeseCounter(int max_count) {
  // First, draw the icon
  for (int i = 0; i < sizeof(CHEESE_ICON) / sizeof(CHEESE_ICON[0]); i++) {
    xy_plot_pixel(5 + CHEESE_ICON[i].x, 220 + CHEESE_ICON[i].y,
                  CHEESE_ICON[i].color);
  }

  // Draw 0/(max_count)
  for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
    xy_plot_pixel(25 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
  }

  for (int i = 0; i < sizeof(SLASH) / sizeof(SLASH[0]); i++) {
    xy_plot_pixel(39 + SLASH[i].x, 220 + SLASH[i].y, 0xFFFF);
  }

  if (max_count == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(49 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (max_count == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(49 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (max_count == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(49 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  }
}

/*******************************************************************************
 * Called when CHEESE_COUNT is updated. Erase previous number and draw new
 ******************************************************************************/
void updateCheeseCounter() {
  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 12; y++) {
      xy_plot_pixel(25 + x, 220 + y, LEVEL1[220 + y][23 + x]);
    }
  }

  if (CHEESE_COUNT == 0) {
    for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
      xy_plot_pixel(25 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(25 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(25 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(25 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(23 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  }
}

/*******************************************************************************
 * Draw initial timer 00:00:00
 ******************************************************************************/
void drawTimer() {
  for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
    xy_plot_pixel(302 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(292 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(272 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(262 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(242 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(232 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
  }

  for (int i = 0; i < sizeof(COLON) / sizeof(COLON[0]); i++) {
    xy_plot_pixel(282 + COLON[i].x, 220 + COLON[i].y, 0xFFFF);
    xy_plot_pixel(252 + COLON[i].x, 220 + COLON[i].y, 0xFFFF);
  }
}

/*******************************************************************************
 * Updates and draws new timer based on DE1-SoC timer
 ******************************************************************************/
void updateTimer() {
  int d1 = 0;  // Most significant minute
  int d2 = 0;
  int d3 = 0;
  int d4 = 0;
  int d5 = 0;
  int d6 = 0;  // Least significant hundreth of a second

  // Isolate each of its values to be printed
  if (centiseconds != 0) {
    d6 = centiseconds % 10;
    d5 = centiseconds / 10;
  }

  if (seconds != 0) {
    d4 = seconds % 10;
    d3 = seconds / 10;
  }

  if (minutes != 0) {
    d2 = minutes % 10;
    d1 = minutes / 10;
  }

  // Reset centiseconds
  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < 12; y++) {
      xy_plot_pixel(292 + x, 220 + y, LEVEL1[220 + y][292 + x]);
    }
  }

  int size5, size6;

  if (d6 == 0) {
    size6 = sizeof(NUM_0) / sizeof(NUM_0[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d6 == 1) {
    size6 = sizeof(NUM_1) / sizeof(NUM_1[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d6 == 2) {
    size6 = sizeof(NUM_2) / sizeof(NUM_2[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d6 == 3) {
    size6 = sizeof(NUM_3) / sizeof(NUM_3[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d6 == 4) {
    size6 = sizeof(NUM_4) / sizeof(NUM_4[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d6 == 5) {
    size6 = sizeof(NUM_5) / sizeof(NUM_5[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d6 == 6) {
    size6 = sizeof(NUM_6) / sizeof(NUM_6[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d6 == 7) {
    size6 = sizeof(NUM_7) / sizeof(NUM_7[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d6 == 8) {
    size6 = sizeof(NUM_8) / sizeof(NUM_8[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d6 == 9) {
    size6 = sizeof(NUM_9) / sizeof(NUM_9[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(302 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }

  if (d5 == 0) {
    size5 = sizeof(NUM_0) / sizeof(NUM_0[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d5 == 1) {
    size5 = sizeof(NUM_1) / sizeof(NUM_1[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d5 == 2) {
    size5 = sizeof(NUM_2) / sizeof(NUM_2[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d5 == 3) {
    size5 = sizeof(NUM_3) / sizeof(NUM_3[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d5 == 4) {
    size5 = sizeof(NUM_4) / sizeof(NUM_4[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d5 == 5) {
    size5 = sizeof(NUM_5) / sizeof(NUM_5[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d5 == 6) {
    size5 = sizeof(NUM_6) / sizeof(NUM_6[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d5 == 7) {
    size5 = sizeof(NUM_7) / sizeof(NUM_7[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d5 == 8) {
    size5 = sizeof(NUM_8) / sizeof(NUM_8[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d5 == 9) {
    size5 = sizeof(NUM_9) / sizeof(NUM_9[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(292 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }
  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < 12; y++) {
      xy_plot_pixel(262 + x, 220 + y, LEVEL1[220 + y][262 + x]);
    }
  }

  if (d4 == 0) {
    for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
      xy_plot_pixel(272 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d4 == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(272 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d4 == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(272 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d4 == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(272 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d4 == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(272 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d4 == 5) {
    for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
      xy_plot_pixel(272 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d4 == 6) {
    for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
      xy_plot_pixel(272 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d4 == 7) {
    for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
      xy_plot_pixel(272 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d4 == 8) {
    for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
      xy_plot_pixel(272 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d4 == 9) {
    for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
      xy_plot_pixel(272 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }

  if (d3 == 0) {
    for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
      xy_plot_pixel(262 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d3 == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(262 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d3 == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(262 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d3 == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(262 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d3 == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(262 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d3 == 5) {
    for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
      xy_plot_pixel(262 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d3 == 6) {
    for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
      xy_plot_pixel(262 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d3 == 7) {
    for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
      xy_plot_pixel(262 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d3 == 8) {
    for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
      xy_plot_pixel(262 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d3 = 9) {
    for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
      xy_plot_pixel(262 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }

  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < 12; y++) {
      xy_plot_pixel(232 + x, 220 + y, LEVEL1[220 + y][232 + x]);
    }
  }

  if (d2 == 0) {
    for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
      xy_plot_pixel(242 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d2 == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(242 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d2 == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(242 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d2 == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(242 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d2 == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(242 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d2 == 5) {
    for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
      xy_plot_pixel(242 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d2 == 6) {
    for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
      xy_plot_pixel(242 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d2 == 7) {
    for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
      xy_plot_pixel(242 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d2 == 8) {
    for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
      xy_plot_pixel(242 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d2 == 9) {
    for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
      xy_plot_pixel(242 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }

  if (d1 == 0) {
    for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
      xy_plot_pixel(232 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d1 == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(232 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d1 == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(232 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d1 == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(232 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d1 == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(232 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d1 == 5) {
    for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
      xy_plot_pixel(232 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d1 == 6) {
    for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
      xy_plot_pixel(232 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d1 == 7) {
    for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
      xy_plot_pixel(232 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d1 == 8) {
    for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
      xy_plot_pixel(232 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d1 == 9) {
    for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
      xy_plot_pixel(232 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }
}

void drawColons() {
  for (int i = 0; i < sizeof(COLON) / sizeof(COLON[0]); i++) {
    xy_plot_pixel(282 + COLON[i].x, 220 + COLON[i].y, 0xFFFF);
    xy_plot_pixel(252 + COLON[i].x, 220 + COLON[i].y, 0XFFFF);
  }
}