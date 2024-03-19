#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

void enableGlobalInterrupts(void) {
    NIOS2_WRITE_ENABLE(0x80);   // Enable interrupts for PS2
    NIOS2_WRITE_STATUS(1);      // Enable Nios II interrupts
}

void PS2_ISR(void) {
    volatile int *PS2_ptr = (int *) PS2_BASE;
    int PS2_data, RAVAIL;

    PS2_data = *(PS2_ptr);
    RAVAIL = (PS2_data & 0xFFFF0000) >> 16;

    if (RAVAIL > 0) {
        byte1 = byte2;
        byte2 = byte3;
        byte3 = PS2_data & 0xFF;

    }

    return;
}



#endif