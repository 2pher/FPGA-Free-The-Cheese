#ifndef DISPLAY_H
#define DISPLAY_H

#include "death_counter.h"
#include "draw.h"
#include "levels.h"
#include "title_screen.h"
#include "utilities.h"

void drawLevelCount(int count);
void drawCheeseCounter(int max_count);
void updateCheeseCounter();

void drawTimer();
void updateTimer();

#endif