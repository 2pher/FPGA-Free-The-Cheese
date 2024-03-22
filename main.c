#include "globalHeader.h"

/* Declare global variables */
extern volatile int pixel_buffer_start;
extern volatile char byte1, byte2, byte3;
extern bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;

/* Function prototypes */
void enableGlobalInterrupts(void);
void configPS2(void);
void display_HEX(char, char, char);
void update_LED(void);

/* Variables for initializtion*/
uint16_t buffer1[240][512];
uint16_t buffer2[240][512];


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

    // Initialize VGA
    *(pixel_ctrl_ptr + 1) = (int) &buffer1;     // Initialize memory in back buffer
    wait_for_vsync();                           // Swap back <-> front buffer
    clear_screen(); /* NOT SURE IF NEEDED; PLAY AROUND */
    *(pixel_ctrl_ptr + 1) = (int) &buffer2;     // "Carve" space in back buffer
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // We draw on back buffer
    clear_screen(); /* NOT SURE IF NEEDED; PLAY AROUND */

    pixel_buffer_start = &BACKGROUND;
    wait_for_vsync();


    // DRAW SOME KIND OF TITLE SCREEN
    // Initialize TITLE_SCREEN in images.h
    uint16_t screen_buff[240][320] = {0};
    /* memcpy(&screen_buff, &TITLE_SCREEN, 240*320*sizeof(uint16_t));
    pixel_buffer_start = &screen_buff; */
    wait_for_vsync();


    //point* initialLocation = pointStruct(50, 50);
    //Square* newSquare = squareStruct(initialLocation, 11);

    while(true){
        //draw_player_square(newSquare);
        //moveSquare(newSquare, 0, 0, 1, 0);
        display_HEX(byte1, byte2, byte3);
        update_LED();
    }
    
}