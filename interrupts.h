#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "display.h"
#include "globalHeader.h"

/*******************************************************************************
 * Global interrupt enabler for board
 ******************************************************************************/
void enableGlobalInterrupts(void);

/*******************************************************************************
 * Interrupt enabler for PS2 controller
 ******************************************************************************/
void configPS2(void);

/*******************************************************************************
 * Interrupt enabler for timer
 ******************************************************************************/
void configTimer(void);

/*******************************************************************************
 * Interrupt handler for PS2 Controller
 ******************************************************************************/
void PS2_ISR(void);

/*******************************************************************************
 * Interrupt handler for Timer
 ******************************************************************************/
void timer_ISR(void);

/*******************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
 ******************************************************************************/
void display_HEX(char b1, char b2, char b3);

/*******************************************************************************
 * Subroutine to update LEDs based on keyboard input
 ******************************************************************************/
void update_LED(void);

//struct for audio device
typedef struct audioDevice {
    volatile unsigned int control;
    volatile unsigned char rarc;
    volatile unsigned char ralc;
    volatile unsigned char wsrc;
    volatile unsigned char wslc;
    volatile unsigned int ldata;
    volatile unsigned int rdata;
} audioDevice;

/*******************************************************************************
 * Subroutine to play a stored audio sample
 ******************************************************************************/
void playAudio(int samples[], int size);

#endif