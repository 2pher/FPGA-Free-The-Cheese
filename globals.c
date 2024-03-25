#include "globalHeader.h"

/*****************************************************************************/
/* Declaring all global variables to be used in the program                  */
/*****************************************************************************/

volatile int pixel_buffer_start;
volatile char byte1, byte2, byte3;          // Saves last 3 bytes of keyboard input
bool KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT; // Bools to check if key pressed
int DEATH_COUNT;                            // Number of user deaths