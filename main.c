#include "background.h"
#include "death_counter.h"
#include "draw.h"
#include "globalHeader.h"
#include "interrupts.h"
#include "shapes.h"

/* Declare global variables */
extern volatile int pixel_buffer_start;
extern volatile char byte1, byte2, byte3;
extern bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;
extern short int buffer1[240][512];  // Store into front buffer
extern short int buffer2[240][512];  // Store into back buffer
extern int DEATH_COUNT;
int OLD_COUNT;

/* Function prototypes */
void drawDeathCounter(void);
void updateDeathCounter(void);
void updateCount(int, int);

/*******************************************************************************
 * Main program of Free Da Cheese
 ******************************************************************************/
int main(void) {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Enable interrupts device-wide and board-wide
  configPS2();
  enableGlobalInterrupts();

  // Initialize global variables
  byte1 = 0;
  byte2 = 0;
  byte3 = 0;
  KEY_UP = false;
  KEY_DOWN = false;
  KEY_LEFT = false;
  KEY_RIGHT = false;
  DEATH_COUNT = 0;
  OLD_COUNT = 0;

  // Initialize VGA
  *(pixel_ctrl_ptr + 1) = (int)&buffer1;  // Initialize memory in back buffer
  wait_for_vsync();                       // Swap back <-> front buffer

  pixel_buffer_start = *pixel_ctrl_ptr;
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, BACKGROUND[y][x]);
    }
  }
  drawDeathCounter();
  updateCount(0, 1);

  *(pixel_ctrl_ptr + 1) = (int)&buffer2;       // "Carve" space in back buffer
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // We draw on back buffer

  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      xy_plot_pixel(x, y, BACKGROUND[y][x]);
    }
  }
  drawDeathCounter();
  updateCount(0, 1);

  point* initialLocation = pointStruct(50, 50);
  Square* newSquare = squareStruct(initialLocation, 11);

  while (1) {
    draw_player_square(newSquare);
    moveSquareNoAcc(newSquare);
    display_HEX(byte1, byte2, byte3);
    update_LED();
    // updateDeathCounter();
    wait_for_vsync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  }

  freePoint(initialLocation);
  freeSquare(newSquare);
}

void drawDeathCounter() {
  for (int i = 0; i < (sizeof(DEATH_COUNTER) / sizeof(DEATH_COUNTER[0])); i++) {
    xy_plot_pixel(DEATH_COUNTER[i].x, DEATH_COUNTER[i].y, 0xFFFF);
  }
}

void updateDeathCounter() {
  // Check if the counter needs to be redrawn
  if (OLD_COUNT == DEATH_COUNT) {
    return;
  }

  // Reset current counter
  for (int x = 0; x < 24; x++) {
    for (int y = 0; y < 20; y++) {
      // Fill it with whatever the background colour was
      xy_plot_pixel(x + 146, y + 7, BACKGROUND[y + 7][x + 146]);
    }
  }

  if (DEATH_COUNT >= 10) {
    // We need to initialize counter for two digits
    updateCount(DEATH_COUNT / 10, 1);  // Extract MSD
    updateCount(DEATH_COUNT % 10, 2);  // Extract LSD
  } else {
    // Do first one only
    updateCount(DEATH_COUNT, 1);
  }
}

void updateCount(int num, int digit) {
  // 146, 158, 7
  if (num == 0) {
    for (int i = 0; i < (sizeof(NUM_0) / sizeof(NUM_0[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_0[i].x + 146, NUM_0[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_0[i].x + 158, NUM_0[i].y + 7, 0xFFFF);
    }
  } else if (num == 1) {
    for (int i = 0; i < (sizeof(NUM_1) / sizeof(NUM_1[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_1[i].x + 146, NUM_1[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_1[i].x + 158, NUM_1[i].y + 7, 0xFFFF);
    }
  } else if (num == 2) {
    for (int i = 0; i < (sizeof(NUM_2) / sizeof(NUM_2[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_2[i].x + 146, NUM_2[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_2[i].x + 158, NUM_2[i].y + 7, 0xFFFF);
    }
  } else if (num == 3) {
    for (int i = 0; i < (sizeof(NUM_3) / sizeof(NUM_3[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_3[i].x + 146, NUM_3[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_3[i].x + 158, NUM_3[i].y + 7, 0xFFFF);
    }
  } else if (num == 4) {
    for (int i = 0; i < (sizeof(NUM_4) / sizeof(NUM_4[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_4[i].x + 146, NUM_4[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_4[i].x + 158, NUM_4[i].y + 7, 0xFFFF);
    }
  } else if (num == 5) {
    for (int i = 0; i < (sizeof(NUM_5) / sizeof(NUM_5[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_5[i].x + 146, NUM_5[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_5[i].x + 158, NUM_5[i].y + 7, 0xFFFF);
    }
  } else if (num == 6) {
    for (int i = 0; i < (sizeof(NUM_6) / sizeof(NUM_6[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_6[i].x + 146, NUM_6[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_6[i].x + 158, NUM_6[i].y + 7, 0xFFFF);
    }
  } else if (num == 7) {
    for (int i = 0; i < (sizeof(NUM_7) / sizeof(NUM_7[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_7[i].x + 146, NUM_7[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_7[i].x + 158, NUM_7[i].y + 7, 0xFFFF);
    }
  } else if (num == 8) {
    for (int i = 0; i < (sizeof(NUM_8) / sizeof(NUM_8[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_8[i].x + 146, NUM_8[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_8[i].x + 158, NUM_8[i].y + 7, 0xFFFF);
    }
  } else {  // num = 9
    for (int i = 0; i < (sizeof(NUM_9) / sizeof(NUM_9[0])); i++) {
      if (digit == 1)
        xy_plot_pixel(NUM_9[i].x + 146, NUM_9[i].y + 7, 0xFFFF);
      else
        xy_plot_pixel(NUM_9[i].x + 158, NUM_9[i].y + 7, 0xFFFF);
    }
  }
}