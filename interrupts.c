#include "interrupts.h"

#include "globalHeader.h"

/* Declare global variables */
extern volatile char byte1, byte2, byte3;
extern bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;
extern bool TITLE_SCREEN;
extern int DEATH_COUNT;  // TO BE REMOVED

/*******************************************************************************
 * Global interrupt enabler for board
 ******************************************************************************/
void enableGlobalInterrupts(void) {
  NIOS2_WRITE_IENABLE(0x80);  // Enable interrupts for PS2 (0x80)
  NIOS2_WRITE_STATUS(1);      // Enable Nios II interrupts
}

/*******************************************************************************
 * Global interrupt enabler for board
 ******************************************************************************/
void configPS2(void) {
  volatile int *PS2_ptr = (int *)PS2_BASE;
  *(PS2_ptr) = 0xFF;     // Reset
  *(PS2_ptr + 1) = 0x1;  // Enable interrupts in PS2 control register
}

/*******************************************************************************
 * Interrupt handler for PS2 Controller
 ******************************************************************************/
void PS2_ISR(void) {
  volatile int *PS2_ptr = (int *)PS2_BASE;
  int PS2_data, RAVAIL;

  // Get PS2 Data
  PS2_data = *(PS2_ptr);
  // AND to check for bit-15 (read data valid)
  RAVAIL = (PS2_data & 0xFFFF0000) >> 16;

  // Valid input detected; do actions
  if (RAVAIL > 0) {
    bool break_code = false;  // Bool to check if key is unpressed

    // Holds last 3 bytes (checks for press/unpress)
    byte1 = byte2;
    byte2 = byte3;
    byte3 = PS2_data & 0xFF;  // Get current data in bits 7:0

    // Check if the keyboard is unpressed
    // byte2 will equal 0xF0!
    if (byte2 == (char)0xF0) break_code = true;

    if (TITLE_SCREEN && break_code && byte3 == (char)0x29) {
      TITLE_SCREEN = false;
    }

    // Update globals.c vars accordingly
    if (byte3 == (char)0x1D) {
      // W key pressed
      if (break_code)
        KEY_UP = false;
      else
        KEY_UP = true;

    } else if (byte3 == (char)0x1C) {
      // A key pressed
      if (break_code)
        KEY_LEFT = false;
      else
        KEY_LEFT = true;

    } else if (byte3 == (char)0x1B) {
      // D key pressed
      if (break_code)
        KEY_RIGHT = false;
      else
        KEY_RIGHT = true;

    } else if (byte3 == (char)0x23) {
      // S key pressed
      if (break_code)
        KEY_DOWN = false;
      else
        KEY_DOWN = true;

    } else if (byte2 != (char)0xF0 && byte2 != (char)0x59 &&
               byte3 == (char)0x59) {
      DEATH_COUNT++;  // TO BE REMOVED
    }
  }

  return;
}

/*******************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
 ******************************************************************************/
void display_HEX(char b1, char b2, char b3) {
  volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
  volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

  /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
   * a single 7-seg display, for the hex digits 0 - F */
  unsigned char seven_seg_decode_table[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,
                                            0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C,
                                            0x39, 0x5E, 0x79, 0x71};
  unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
  unsigned int shift_buffer, nibble;
  unsigned char code;
  int i;

  shift_buffer = (b1 << 16) | (b2 << 8) | b3;
  for (i = 0; i < 6; ++i) {
    nibble = shift_buffer & 0x0000000F;  // Character is in rightmost nibble
    code = seven_seg_decode_table[nibble];
    hex_segs[i] = code;
    shift_buffer = shift_buffer >> 4;
  }
  /* drive the hex displays */
  *(HEX3_HEX0_ptr) = *(int *)(hex_segs);
  *(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
}

/*******************************************************************************
 * Subroutine to update LEDs based on keyboard input
 ******************************************************************************/
void update_LED(void) {
  volatile int *LED_ptr = (int *)LED_BASE;
  *(LED_ptr) = 0x0;

  // Should update the LEDs accordingly if global variable true
  if (KEY_UP) *(LED_ptr) = 0x8;
  if (KEY_LEFT) *(LED_ptr) = 0x4;
  if (KEY_RIGHT) *(LED_ptr) = 0x2;
  if (KEY_DOWN) *(LED_ptr) = 0x1;
}