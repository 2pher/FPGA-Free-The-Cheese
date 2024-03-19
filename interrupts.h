#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

bool UP, DOWN, LEFT, RIGHT;

void enableGlobalInterrupts(void);
void PS2_ISR(void);
void display_HEX(char, char, char);

void enableGlobalInterrupts(void) {
    NIOS2_WRITE_ENABLE(0x80);   // Enable interrupts for PS2
    NIOS2_WRITE_STATUS(1);      // Enable Nios II interrupts
}

void PS2_ISR(void) {
    volatile int *PS2_ptr = (int *) PS2_BASE;
    int PS2_data, RAVAIL;

    PS2_data = *(PS2_ptr);
    RAVAIL = (PS2_data & 0xFFFF0000) >> 16;

    // Valid input detected
    if (RAVAIL > 0) {
        bool break_code = false;

        byte1 = byte2;
        byte2 = byte3;
        byte3 = PS2_data & 0xFF;

        // Check if the keyboard is unpressed
        // byte2 will equal 0xF0!
        if (byte2 == (char) 0xF0) break_code = true;
        
        // Update globals vars accordingly
        if (byte3 == (char) 0x1D) {
            if (break_code) UP = false; 
            else UP = true;

        } else if (byte3 == (char) 0x1C) {
            if (break_code) LEFT = false; 
            else LEFT = true;

        } else if (byte3 == (char) 0x1B) {
            if (break_code) RIGHT = false; 
            else RIGHT = true;

        } else if (byte3 == (char) 0x1B) {
            if (break_code) DOWN = false; 
            else DOWN = true;
        }

    }
    
    return;
}

void display_HEX(char b1, char b2, char b3) {
   volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
   volatile int * HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
     * a single 7-seg display, for the hex digits 0 - F */
    unsigned char seven_seg_decode_table[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int  shift_buffer, nibble;
    unsigned char code;
    int           i;

    shift_buffer = (b1 << 16) | (b2 << 8) | b3;
    for (i = 0; i < 6; ++i) {
        nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
        code   = seven_seg_decode_table[nibble];
        hex_segs[i]  = code;
        shift_buffer = shift_buffer >> 4;
    }
    /* drive the hex displays */
    *(HEX3_HEX0_ptr) = *(int *)(hex_segs);
    *(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
}

void update_LED(void) {
    volatile int * LED_ptr = (int *)LED_BASE;

    *(LED_ptr) = 0x0;

    if (UP) *(LED_ptr) = 0x8;
    if (LEFT) *(LED_ptr) = 0x4;
    if (RIGHT) *(LED_ptr) = 0x2;
    if (DOWN) *(LED_ptr) = 0x1;

}

#endif