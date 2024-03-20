#include "globalHeader.h"

// Define variables in globals.c
extern volatile char byte1, byte2, byte3;
extern bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;

/*******************************************************************************
 * Main program of Free Da Cheese
 ******************************************************************************/
int main() {

    volatile int* pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    
    // Enable interrupts device-wide and board-wide
    enableInterrupts();
    
    // Initialize some variables
    byte1 = 0;
    byte2 = 0;
    byte3 = 0;
    KEY_UP = false;
    KEY_DOWN = false;
    KEY_LEFT = false;
    KEY_RIGHT = false;

    point* initialLocation = pointStruct(50, 50);
    Square* newSquare = squareStruct(initialLocation, 11);

    while(true){
        draw_player_square(newSquare);
        moveSquare(newSquare, 0, 0, 1, 0);
        display_HEX(byte1, byte2, byte3);
        update_LED();
    }
    
}