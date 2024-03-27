#include "globalHeader.h"

/*****************************************************************************/
/* Declaring all global variables to be used in the program                  */
/*****************************************************************************/

volatile int pixel_buffer_start;
volatile char byte1, byte2, byte3;  // Saves last 3 bytes of keyboard input
bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT;  // Bools to check if key pressed
bool TITLE_SCREEN;                           // Title screen checker
int DEATH_COUNT;                             // Number of user deaths
short int buffer1[240][512];                 // Store into front buffer
short int buffer2[240][512];                 // Store into back buffer