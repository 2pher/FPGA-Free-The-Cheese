#include "display.h"

extern volatile int pixel_buffer_start;
extern int CHEESE_COUNT;

void drawCheeseCounter(int max_count) {
  // First, draw the icon
  for (int i = 0; i < sizeof(CHEESE_ICON) / sizeof(CHEESE_ICON[0]); i++) {
    xy_plot_pixel(5 + CHEESE_ICON[i].x, 234 + CHEESE_ICON[i].y,
                  CHEESE_ICON[i].color);
  }

  // Draw 0/(max_count)
  for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
    xy_plot_pixel(19 + NUM_0[i].x, 232 + NUM_0[i].y, 0xFFFF);
  }

  for (int i = 0; i < sizeof(SLASH) / sizeof(SLASH[0]); i++) {
    xy_plot_pixel(31 + SLASH[i].x, 232 + SLASH[i].y, 0xFFFF);
  }

  if (max_count == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3); i++) {
      xy_plot_pixel(43 + NUM_3[i].x, 232 + NUM_3[i].y, 0xFFFF);
    }
  } else if (max_count == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4); i++) {
      xy_plot_pixel(43 + NUM_4[i].x, 232 + NUM_4[i].y, 0xFFFF);
    }
  }
}

void updateCheeseCounter() {
  for (int x = 0; x < 12; x++) {
    for (int y = 0; y < 10; y++) {
      xy_plot_pixel(31 + x, 232 + y, LEVEL1[232 + y][31 + x]);
    }
  }

  if (CHEESE_COUNT == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(19 + NUM_1[i].x, 232 + NUM_1[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(19 + NUM_2[i].x, 232 + NUM_2[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(19 + NUM_3[i].x, 232 + NUM_3[i].y, 0xFFFF);
    }
  } else if (CHEESE_COUNT == 4) {
    for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
      xy_plot_pixel(19 + NUM_4[i].x, 232 + NUM_4[i].y, 0xFFFF);
    }
  }
}