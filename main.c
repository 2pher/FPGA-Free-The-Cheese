#include "globals.h"

int main() {

    volatile int* pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    enableInterrupts();

    point* initialLocation = pointStruct(50, 50);
    Square* newSquare = squareStruct(initialLocation, 11);

    while(true){
        draw_player_square(newSquare);
        moveSquare(newSquare, 0, 0, 1, 0);
    }
    
}