#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "utilities.h"
#include "shapes.h"
#include "draw.h"


/* Cyclone V FPGA devices */
#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050
#define TIMER_BASE 0xFF202000
#define PS2_CONTROLLER_BASE 0xFF200100
#define PIXEL_BUF_CTRL_BASE 0xFF203020
#define CHAR_BUF_CTRL_BASE 0xFF203030

/* Control registers to enable interrupts */
#define CTL0 0x1C000000
#define CTL3 0x1C00000C

extern int pixel_buffer_start;

extern bool LEFT, RIGHT, UP, DOWN;


#endif