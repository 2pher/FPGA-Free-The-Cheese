#include "death_counter.h"
#include "draw.h"
#include "globalHeader.h"
#include "interrupts.h"
#include "levels.h"
#include "shapes.h"
#include "title_screen.h"

/* Declare global variables */
extern volatile int pixel_buffer_start;
extern volatile char byte1, byte2, byte3;
extern bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;
extern bool ON_TITLE_SCREEN, ON_LEVEL1;
extern short int buffer1[240][512];  // Store into front buffer
extern short int buffer2[240][512];  // Store into back buffer
extern int DEATH_COUNT;
int OLD_COUNT;

/* Function prototypes */
void configVGA(void);
void updateTitleScreen(void);
// void resetBackground(void);
void drawDeathCounter(void);
void updateDeathCounter(void);
void updateCount(int, int);
void configLevel1(void);
void checkWin(Square*, int);

/*******************************************************************************
 *  FREE THE CHEESE : MAIN PROGRAM
 ******************************************************************************/
int main(void) {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Enable interrupts device-wide and board-wide
  configPS2();               // Enable PS2 interrupts
  configVGA();               // Enable double buffering, draw title screen
  enableGlobalInterrupts();  // Enable global interrupts for DE1-SoC board

  // Initialize global variables
  byte1 = 0;               // PS2 keyboard 3rd-newest byte
  byte2 = 0;               // PS2 keyboard 2nd-newest byte
  byte3 = 0;               // PS2 keyboard newest byte
  KEY_UP = false;          // Global variable for moving up
  KEY_DOWN = false;        // Global variable for moving down
  KEY_LEFT = false;        // Global variable for moving right
  KEY_RIGHT = false;       // Global variable for moving left
  ON_TITLE_SCREEN = true;  // Global variable that stays true until <SPACE>
  DEATH_COUNT = 0;         // Initialize user death count
  OLD_COUNT = 0;           // Old death count to compare to current
  ON_LEVEL1 = true;

  /*******************************************************************************
   *  TITLE SCREEN
   ******************************************************************************/
  while (ON_TITLE_SCREEN) {
    // Animate the mouse until user presses enter
    updateTitleScreen();
  }

  /*******************************************************************************
   *  LEVEL 1
   ******************************************************************************/
  configLevel1();  // Print new background and level on both buffer frames

  // Initialize starting square
  point* initialLocation = pointStruct(42, 115);
  Square* newSquare = squareStruct(initialLocation, 9);
  point* oldSquare;
  point* prevSquare;
  prevSquare = newSquare->position;
  oldSquare = newSquare->position;

  // Initialize all of the bots, create an array of them
  Circle* e1 = circleStruct(pointStruct(76, 45), 3, pointStruct(0, 3), NULL);
  Circle* e2 = circleStruct(pointSturct(140, 45), 3, pointStruct(0, 3), NULL);
  Circle* e3 = circleStruct(pointStruct(204, 45), 3, pointStruct(0, 3), NULL);
  Circle* e4 = circleStruct(pointStruct(108, 202), 3, pointStruct(0, -3), NULL);
  Circle* e5 = circleStruct(pointStruct(172, 202), 3, pointStruct(0, -3), NULL);
  Circle* e6 = circleStruct(pointStruct(236, 202), 3, pointStruct(0, -3), NULL);
  Circle* enemies[] = {e1, e2, e3, e4, e5, e6};

  point* oldEnemies[6];
  point* prevEnemies[6];
  for (int i = 0; i < 6; i++) {
    prevEnemies[i] = enemies[i]->position;
    oldEnemies[i] = enemies[i]->position;
  }

  // Level 1 main loop
  while (ON_LEVEL1) {
    // Calculate position
    moveSquareNoAcc(newSquare);
    // Calculate position of each enemy
    moveCircle(enemies);
    // Check for any collisions
    checkForCollisions(newSquare, enemies);
    // Check if won
    checkWin(newSquare, 1);
    // Erase player square if necessary
    erase_player_square(oldSquare, newSquare, 1);
    // Erase old circle positions, draw new ones
    drawCircle(enemies, oldEnemies, 1);
    // Draw player's current position
    draw_player_square(newSquare);

    // Update oldSquare position
    oldSquare = prevSquare;
    prevSquare = newSquare->position;

    // Update circle positions
    for (int i = 0; i < 6; i++) {
      oldEnemies[i] = prevEnemies[i];
      prevEnemies[i] = enemies[i]->position;
    }

    display_HEX(byte1, byte2, byte3);
    update_LED();

    // Update death counter if necessary
    updateDeathCounter();

    wait_for_vsync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  }

  // End of level 1; deallocate all pointers
  freePoint(initialLocation);
  freeSquare(newSquare);

  /*******************************************************************************
   *  LEVEL 2
   ******************************************************************************/
}

void configVGA() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Initialize VGA
  *(pixel_ctrl_ptr + 1) = (int)&buffer1;  // Initialize memory in back buffer
  wait_for_vsync();                       // Swap back <-> front buffer

  pixel_buffer_start = *pixel_ctrl_ptr;
  drawTitleScreen();

  *(pixel_ctrl_ptr + 1) = (int)&buffer2;       // "Carve" space in back buffer
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // We draw on back buffer

  drawTitleScreen();
  wait_for_vsync();
}

void updateTitleScreen() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Erase mouse 3, draw mouse 1, wait for 2 seconds
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  drawMouse(3, false);
  drawMouse(1, true);
  wait_for_vsync();
  int counter = 0;
  while (counter != 75000000) {
    counter++;
  }

  // Erase mouse 1, draw mouse 2, wait for 0.1 seconds
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  drawMouse(2, false);
  drawMouse(2, true);
  wait_for_vsync();
  counter = 0;
  while (counter != 5000000) counter++;

  // Erase mouse 2, draw mouse 3, wait for 2 seconds
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  drawMouse(1, false);
  drawMouse(3, true);
  wait_for_vsync();
  counter = 0;
  while (counter != 75000000) counter++;

  // Erase mouse 3
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  drawMouse(2, false);
  drawMouse(2, true);
  wait_for_vsync();
  counter = 0;
  while (counter != 5000000) counter++;
}

/* void resetBackground() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Draw background on back buffer
  drawBackground();
  drawDeathCounter();
  updateCount(0, 1);
  wait_for_vsync();                            // Send to front
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // Get new back buffer pointer

  // Draw background on back buffer again to "reset" both frames
  drawBackground();
  drawDeathCounter();
  updateCount(0, 1);
} */

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
      xy_plot_pixel(x + 146, y + 7, LEVEL1[y + 7][x + 146]);
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
  OLD_COUNT = DEATH_COUNT;
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

void configLevel1() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Draw background & level on back buffer
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  drawLevel1();
  drawDeathCounter();
  updateCount(0, 1);
  wait_for_vsync();                            // Send to front
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // Get new back buffer pointer

  // Draw background on back buffer again to "reset" both frames
  drawLevel1();
  drawDeathCounter();
  updateCount(0, 1);
}

void checkWin(Square* newSquare, int level) {
  if (level == 1) {
    if (newSquare->position->x >= 270 && newSquare->position->y >= 112 &&
        newSquare->position->y <= 136) {
      // IN THE POSITION OF A WIN!!!
      ON_LEVEL1 = false;
    }
  }
}