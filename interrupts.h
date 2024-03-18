#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "globals.h"

void enableGlobalInterrupts() {
    
}

void enableInterrupts() {
    volatile int *ps2_ctrl_ptr = PS2_CONTROLLER_BASE;

    // Enable PS2 controller interrupt
    *(ps2_ctrl_ptr + 1) = 0x1;
}

#endif