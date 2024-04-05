#include "death_counter.h"
#include "display.h"
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
extern bool ON_TITLE_SCREEN, ON_LEVEL1, ON_LEVEL2;
extern short int buffer1[240][512];  // Store into front buffer
extern short int buffer2[240][512];  // Store into back buffer
extern int DEATH_COUNT, CHEESE_COUNT;
extern bool level1, level2;
int OLD_COUNT1, OLD_COUNT2;

/* Function prototypes */
void configVGA(void);
void updateTitleScreen(void);
// void resetBackground(void);
void drawDeathCounter(void);
void updateDeathCounter(void);
void updateCount(int, int);
void configLevel1(void);
void configLevel2(void);
void checkWin(Square*, int, Cheese*[], int);

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
  OLD_COUNT1 = 0;          // Old death count to compare to current
  OLD_COUNT2 = 0;
  ON_LEVEL1 = true;
  ON_LEVEL2 = true;
  level1 = false;
  level2 = false;
  CHEESE_COUNT = 0;

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
  level1 = true;

  // Initialize starting square
  point* initialLocation = pointStruct(42, 115);
  Square* newSquare = squareStruct(initialLocation, 9);
  point* oldSquare;
  point* prevSquare;
  prevSquare = newSquare->position;
  oldSquare = newSquare->position;

  // Initialize all of the bots, create an array of them
  point* p1 = pointStruct(76, 45);
  point* p2 = pointStruct(140, 45);
  point* p3 = pointStruct(204, 45);
  point* p4 = pointStruct(108, 202);
  point* p5 = pointStruct(172, 202);
  point* p6 = pointStruct(236, 202);
  point* down = pointStruct(0, 3);
  point* up = pointStruct(0, -3);
  point* left = pointStruct(-3, 0);
  point* right = pointStruct(3, 0);

  Circle* e1 = circleStruct(p1, 3, down);
  Circle* e2 = circleStruct(p2, 3, down);
  Circle* e3 = circleStruct(p3, 3, down);
  Circle* e4 = circleStruct(p4, 3, up);
  Circle* e5 = circleStruct(p5, 3, up);
  Circle* e6 = circleStruct(p6, 3, up);
  Circle* enemies[] = {e1, e2, e3, e4, e5, e6};

  point* oldEnemies[10];
  point* prevEnemies[10];
  for (int i = 0; i < 6; i++) {
    prevEnemies[i] = enemies[i]->position;
    oldEnemies[i] = enemies[i]->position;
  }

  point* cp1 = pointStruct(59, 39);
  point* cp2 = pointStruct(262, 39);
  point* cp3 = pointStruct(262, 191);
  point* cp4 = pointStruct(59, 191);

  Cheese* c1 = cheeseStruct(cp1);
  Cheese* c2 = cheeseStruct(cp2);
  Cheese* c3 = cheeseStruct(cp3);
  Cheese* c4 = cheeseStruct(cp4);

  Cheese* cheeses[] = {c1, c2, c3, c4};
  for (int i = 0; i < 4; i++) {
    draw_cheese(cheeses[i]);
  }
  wait_for_vsync();
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  for (int i = 0; i < 4; i++) {
    draw_cheese(cheeses[i]);
  }

  // Level 1 main loop
  while (ON_LEVEL1) {
    // Calculate position
    moveSquareNoAcc(newSquare);
    // Calculate position of each enemy
    moveCircles(enemies, 6);
    // Check for any collisions
    checkForCollisions(newSquare, enemies, 6);
    // Check if we collected THE CHEESE!
    checkForCheese(newSquare, cheeses, 4);
    // Check if won
    checkWin(newSquare, 1, cheeses, 4);
    // Erase player square if necessary
    erase_player_square(oldSquare, newSquare, 1);
    // Erase old circle positions, draw new ones
    drawCircles(enemies, oldEnemies, 6, 1);
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
  level1 = false;

  /*******************************************************************************
   *  LEVEL 2
   ******************************************************************************/
  configLevel2();
  level2 = true;

  // Initialize starting square
  point* initialLocation2 = pointStruct(33, 80);
  Square* newSquare2 = squareStruct(initialLocation2, 9);
  prevSquare = newSquare2->position;
  oldSquare = newSquare2->position;

  // Initialize all of the bots, create an array of them
  point* q1 = pointStruct(72, 125);
  point* q2 = pointStruct(72, 135);
  point* q3 = pointStruct(72, 145);
  point* q4 = pointStruct(72, 155);
  point* q5 = pointStruct(80, 157);
  point* q6 = pointStruct(90, 157);
  point* q7 = pointStruct(100, 157);
  point* q8 = pointStruct(110, 157);
  point* q9 = pointStruct(120, 157);
  point* q10 = pointStruct(130, 157);

  Circle* f1 = circleStruct(q1, 5, up);
  Circle* f2 = circleStruct(q2, 5, up);
  Circle* f3 = circleStruct(q3, 5, up);
  Circle* f4 = circleStruct(q4, 5, up);
  Circle* f5 = circleStruct(q5, 5, left);
  Circle* f6 = circleStruct(q6, 5, left);
  Circle* f7 = circleStruct(q7, 5, left);
  Circle* f8 = circleStruct(q8, 5, left);
  Circle* f9 = circleStruct(q9, 5, left);
  Circle* f10 = circleStruct(q10, 5, left);
  Circle* enemies2[] = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10};

  for (int i = 0; i < 10; i++) {
    oldEnemies[i] = enemies2[i]->position;
    prevEnemies[i] = enemies2[i]->position;
  }

  CHEESE_COUNT = 0;
  point* dp1 = pointStruct(249, 53);
  point* dp2 = pointStruct(283, 159);
  point* dp3 = pointStruct(85, 185);

  Cheese* d1 = cheeseStruct(dp1);
  Cheese* d2 = cheeseStruct(dp2);
  Cheese* d3 = cheeseStruct(dp3);

  Cheese* cheeses2[] = {d1, d2, d3};
  for (int i = 0; i < 3; i++) {
    draw_cheese(cheeses2[i]);
  }
  wait_for_vsync();
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  for (int i = 0; i < 3; i++) {
    draw_cheese(cheeses2[i]);
  }

  while (ON_LEVEL2) {
    // Calculate position
    moveSquareNoAcc(newSquare2);
    // Calculate position of each enemy
    moveCircles2(enemies2, 10);
    // Check for any collisions
    checkForCollisions(newSquare2, enemies2, 10);
    // Check for the CHEESE!
    checkForCheese(newSquare2, cheeses2, 3);
    // Check if won
    checkWin(newSquare, 2, cheeses2, 3);
    // Erase player square if necessary
    erase_player_square(oldSquare, newSquare2, 2);
    // Erase old circle positions, draw new ones
    drawCircles(enemies2, oldEnemies, 10, 2);
    // Draw player's current position
    draw_player_square(newSquare2);

    // Update oldSquare position
    oldSquare = prevSquare;
    prevSquare = newSquare2->position;

    // Update circle positions
    for (int i = 0; i < 10; i++) {
      oldEnemies[i] = prevEnemies[i];
      prevEnemies[i] = enemies2[i]->position;
    }

    display_HEX(byte1, byte2, byte3);
    update_LED();

    // Update death counter if necessary
    updateDeathCounter();

    wait_for_vsync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  }
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
  if (OLD_COUNT1 != DEATH_COUNT) {
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
    OLD_COUNT1 = DEATH_COUNT;
    return;
  }

  if (OLD_COUNT2 != DEATH_COUNT) {
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
    OLD_COUNT2 = DEATH_COUNT;
    return;
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

void configLevel1() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Draw background & level on back buffer
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  drawLevel1();
  drawDeathCounter();
  updateCount(0, 1);
  drawCheeseCounter(4);
  wait_for_vsync();                            // Send to front
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // Get new back buffer pointer

  // Draw background on back buffer again to "reset" both frames
  drawLevel1();
  drawDeathCounter();
  drawCheeseCounter(4);
  updateCount(0, 1);
}

void configLevel2() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  OLD_COUNT1 = -1;
  OLD_COUNT2 = -1;
  // Draw background & level on back buffer
  drawLevel2();
  drawDeathCounter();
  updateDeathCounter();
  drawCheeseCounter(3);
  wait_for_vsync();                            // Send to front
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // Get new back buffer pointer

  // Draw background on back buffer again to "reset" both frames
  drawLevel2();
  drawDeathCounter();
  updateDeathCounter();
  drawCheeseCounter(3);
}

void checkWin(Square* newSquare, int level, Cheese* cheeses[], int size) {
  for (int i = 0; i < size; i++) {
    if (cheeses[i]->erasedTwice == false) return;
  }

  int half_side_length = (newSquare->sideLength - 1) / 2;
  if (level == 1) {
    if (newSquare->position->x >= 274 && newSquare->position->y >= 112 &&
        newSquare->position->y <= 136) {
      // IN THE POSITION OF A WIN!!!
      ON_LEVEL1 = false;
    }
  } else if (level == 2) {
    if (newSquare->position->y + half_side_length <= 65 ||
        newSquare->position->y - half_side_length >= 174 ||
        newSquare->position->x + half_side_length <= 53 ||
        newSquare->position->x - half_side_length >= 266) {
      ON_LEVEL2 = false;
    }
  }
}