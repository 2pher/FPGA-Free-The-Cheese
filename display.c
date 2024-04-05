#include "display.h"

extern volatile int pixel_buffer_start;
extern int CHEESE_COUNT;
extern int centiseconds, seconds, minutes;
extern int old_seconds, old_minutes;

void drawLevelCount(int count) {
  for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
    xy_plot_pixel(NUM_3[i].x + 300, NUM_3[i].y + 7, 0xFFFF);
  }

  for (int i = 0; i < sizeof(SLASH) / sizeof(SLASH[0]); i++) {
    xy_plot_pixel(SLASH[i].x + 288, SLASH[i].y + 7, 0xFFFF);
  }

  if (count == 1) {
    for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
      xy_plot_pixel(NUM_1[i].x + 266, NUM_1[i].y + 7, 0xFFFF);
    }
  } else if (count == 2) {
    for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
      xy_plot_pixel(NUM_2[i].x + 266, NUM_2[i].y + 7, 0xFFFF);
    }
  } else if (count == 3) {
    for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
      xy_plot_pixel(NUM_3[i].x + 266, NUM_3[i].y + 7, 0xFFFF);
    }
  }
}

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

void drawTimer() {
  for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
    xy_plot_pixel(300 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(288 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(264 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(252 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(228 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    xy_plot_pixel(216 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
  }

  for (int i = 0; i < sizeof(COLON) / sizeof(COLON[0]); i++) {
    xy_plot_pixel(276 + COLON[i].x, 220 + COLON[i].y, 0xFFFF);
    xy_plot_pixel(240 + COLON[i].x, 220 + COLON[i].y, 0xFFFF);
  }
}

void updateTimer() {
  int d1 = 0;
  int d2 = 0;
  int d3 = 0;
  int d4 = 0;
  int d5 = 0;
  int d6 = 0;

  if (centiseconds != 0) {
    d6 = centiseconds % 10;
    d5 = centiseconds / 10;
  }

  if (seconds != 0) {
    d4 = seconds / 10;
    d3 = seconds / 10;
  }

  if (minutes != 0) {
    d2 = minutes / 10;
    d1 = minutes / 10;
  }

  // Reset centiseconds
  for (int x = 0; x < 24; x++) {
    for (int y = 0; y < 10; y++) {
      xy_plot_pixeL(288 + x, 222 + y, LEVEL1[288 + y][222 + x]);
    }
  }

  int size5, size6;

  if (d6 == 0) {
    size6 = sizeof(NUM_0) / sizeof(NUM_0[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d6 == 1) {
    size6 = sizeof(NUM_1) / sizeof(NUM_1[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d6 == 2) {
    size6 = sizeof(NUM_2) / sizeof(NUM_2[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d6 == 3) {
    size6 = sizeof(NUM_3) / sizeof(NUM_3[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d6 == 4) {
    size6 = sizeof(NUM_4) / sizeof(NUM_4[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d6 == 5) {
    size6 = sizeof(NUM_5) / sizeof(NUM_5[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d6 == 6) {
    size6 = sizeof(NUM_6) / sizeof(NUM_6[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d6 == 7) {
    size6 = sizeof(NUM_7) / sizeof(NUM_7[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d6 == 8) {
    size6 = sizeof(NUM_8) / sizeof(NUM_8[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d6 == 9) {
    size6 = sizeof(NUM_9) / sizeof(NUM_9[0]);
    for (int i = 0; i < size6; i++) {
      xy_plot_pixel(330 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }

  if (d5 == 0) {
    size5 = sizeof(NUM_0) / sizeof(NUM_0[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
    }
  } else if (d5 == 1) {
    size5 = sizeof(NUM_1) / sizeof(NUM_1[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
    }
  } else if (d5 == 2) {
    size5 = sizeof(NUM_2) / sizeof(NUM_2[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
    }
  } else if (d5 == 3) {
    size5 = sizeof(NUM_3) / sizeof(NUM_3[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
    }
  } else if (d5 == 4) {
    size5 = sizeof(NUM_4) / sizeof(NUM_4[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
    }
  } else if (d5 == 5) {
    size5 = sizeof(NUM_5) / sizeof(NUM_5[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
    }
  } else if (d5 == 6) {
    size5 = sizeof(NUM_6) / sizeof(NUM_6[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
    }
  } else if (d5 == 7) {
    size5 = sizeof(NUM_7) / sizeof(NUM_7[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
    }
  } else if (d5 == 8) {
    size5 = sizeof(NUM_8) / sizeof(NUM_8[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
    }
  } else if (d5 == 9) {
    size5 = sizeof(NUM_9) / sizeof(NUM_9[0]);
    for (int i = 0; i < size5; i++) {
      xy_plot_pixel(288 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
    }
  }

  if (old_seconds != seconds) {
    for (int x = 0; x < 24; x++) {
      for (int y = 0; y < 10; y++) {
        xy_plot_pixel(252 + x, 220 + y, LEVEL1[220 + y][252 + x]);
      }
    }

    if (d4 == 0) {
      for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
        xy_plot_pixel(264 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
      }
    } else if (d4 = 1) {
      for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
        xy_plot_pixel(264 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
      }
    } else if (d4 = 2) {
      for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
        xy_plot_pixel(264 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
      }
    } else if (d4 = 3) {
      for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
        xy_plot_pixel(264 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
      }
    } else if (d4 = 4) {
      for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
        xy_plot_pixel(264 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
      }
    } else if (d4 = 5) {
      for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
        xy_plot_pixel(264 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
      }
    } else if (d4 = 6) {
      for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
        xy_plot_pixel(264 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
      }
    } else if (d4 = 7) {
      for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
        xy_plot_pixel(264 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
      }
    } else if (d4 = 8) {
      for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
        xy_plot_pixel(264 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
      }
    } else if (d4 = 9) {
      for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
        xy_plot_pixel(264 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
      }
    }

    if (d3 == 0) {
      for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
        xy_plot_pixel(252 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
      }
    } else if (d3 = 1) {
      for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
        xy_plot_pixel(252 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
      }
    } else if (d3 = 2) {
      for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
        xy_plot_pixel(252 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
      }
    } else if (d3 = 3) {
      for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
        xy_plot_pixel(252 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
      }
    } else if (d3 = 4) {
      for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
        xy_plot_pixel(252 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
      }
    } else if (d3 = 5) {
      for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
        xy_plot_pixel(252 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
      }
    } else if (d3 = 6) {
      for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
        xy_plot_pixel(252 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
      }
    } else if (d3 = 7) {
      for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
        xy_plot_pixel(252 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
      }
    } else if (d3 = 8) {
      for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
        xy_plot_pixel(252 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
      }
    } else if (d3 = 9) {
      for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
        xy_plot_pixel(252 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
      }
    }

    old_seconds = seconds;
  }

  if (old_minutes != minutes) {
    for (int x = 0; x < 24; x++) {
      for (int y = 0; y < 10; y++) {
        xy_plot_pixel(216 + x, 220 + y, LEVEL1[220 + y][252 + x]);
      }
    }

    if (d2 == 0) {
      for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
        xy_plot_pixel(228 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
      }
    } else if (d2 = 1) {
      for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
        xy_plot_pixel(228 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
      }
    } else if (d2 = 2) {
      for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
        xy_plot_pixel(228 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
      }
    } else if (d2 = 3) {
      for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
        xy_plot_pixel(228 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
      }
    } else if (d2 = 4) {
      for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
        xy_plot_pixel(228 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
      }
    } else if (d2 = 5) {
      for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
        xy_plot_pixel(228 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
      }
    } else if (d2 = 6) {
      for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
        xy_plot_pixel(228 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
      }
    } else if (d2 = 7) {
      for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
        xy_plot_pixel(228 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
      }
    } else if (d2 = 8) {
      for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
        xy_plot_pixel(228 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
      }
    } else if (d2 = 9) {
      for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
        xy_plot_pixel(228 + NUM_9[i].x, 220 + NUM_4[i].y, 0xFFFF);
      }
    }

    if (d1 == 0) {
      for (int i = 0; i < sizeof(NUM_0) / sizeof(NUM_0[0]); i++) {
        xy_plot_pixel(216 + NUM_0[i].x, 220 + NUM_0[i].y, 0xFFFF);
      }
    } else if (d1 = 1) {
      for (int i = 0; i < sizeof(NUM_1) / sizeof(NUM_1[0]); i++) {
        xy_plot_pixel(216 + NUM_1[i].x, 220 + NUM_1[i].y, 0xFFFF);
      }
    } else if (d1 = 2) {
      for (int i = 0; i < sizeof(NUM_2) / sizeof(NUM_2[0]); i++) {
        xy_plot_pixel(216 + NUM_2[i].x, 220 + NUM_2[i].y, 0xFFFF);
      }
    } else if (d1 = 3) {
      for (int i = 0; i < sizeof(NUM_3) / sizeof(NUM_3[0]); i++) {
        xy_plot_pixel(216 + NUM_3[i].x, 220 + NUM_3[i].y, 0xFFFF);
      }
    } else if (d1 = 4) {
      for (int i = 0; i < sizeof(NUM_4) / sizeof(NUM_4[0]); i++) {
        xy_plot_pixel(216 + NUM_4[i].x, 220 + NUM_4[i].y, 0xFFFF);
      }
    } else if (d1 = 5) {
      for (int i = 0; i < sizeof(NUM_5) / sizeof(NUM_5[0]); i++) {
        xy_plot_pixel(216 + NUM_5[i].x, 220 + NUM_5[i].y, 0xFFFF);
      }
    } else if (d1 = 6) {
      for (int i = 0; i < sizeof(NUM_6) / sizeof(NUM_6[0]); i++) {
        xy_plot_pixel(216 + NUM_6[i].x, 220 + NUM_6[i].y, 0xFFFF);
      }
    } else if (d1 = 7) {
      for (int i = 0; i < sizeof(NUM_7) / sizeof(NUM_7[0]); i++) {
        xy_plot_pixel(216 + NUM_7[i].x, 220 + NUM_7[i].y, 0xFFFF);
      }
    } else if (d1 = 8) {
      for (int i = 0; i < sizeof(NUM_8) / sizeof(NUM_8[0]); i++) {
        xy_plot_pixel(216 + NUM_8[i].x, 220 + NUM_8[i].y, 0xFFFF);
      }
    } else if (d1 = 9) {
      for (int i = 0; i < sizeof(NUM_9) / sizeof(NUM_9[0]); i++) {
        xy_plot_pixel(216 + NUM_9[i].x, 220 + NUM_9[i].y, 0xFFFF);
      }
    }

    old_minutes = minutes;
  }
}