#include "globalHeader.h"

/* Declare global variables */
extern volatile char byte1, byte2, byte3;
extern bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;

/* Function prototypes */
void enableGlobalInterrupts(void);
void configPS2(void);
void display_HEX(char, char, char);
void update_LED(void);

/*******************************************************************************
 * Main program of Free Da Cheese
 ******************************************************************************/
int main(void) {

    volatile int* pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    
    // Enable interrupts device-wide and board-wide
    configPS2();
    enableInterrupts();
    
    // Initialize global variables
    byte1 = 0;
    byte2 = 0;
    byte3 = 0;
    KEY_UP = false;
    KEY_DOWN = false;
    KEY_LEFT = false;
    KEY_RIGHT = false;

    //point* initialLocation = pointStruct(50, 50);
    //Square* newSquare = squareStruct(initialLocation, 11);

    while(true){
        //draw_player_square(newSquare);
        //moveSquare(newSquare, 0, 0, 1, 0);
        display_HEX(byte1, byte2, byte3);
        update_LED();
    }
    
}