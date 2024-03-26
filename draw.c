#include "draw.h"

extern volatile int pixel_buffer_start;

//plot 1 pixel given xy and colour
void xy_plot_pixel(int x, int y, short int line_color){
	
	volatile short int *one_pixel_address;
	one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
	*one_pixel_address = line_color;
}
//plot 1 pixel given location and colour
void point_plot_pixel(point* location, short int line_color){

    volatile short int *one_pixel_address;
	one_pixel_address = pixel_buffer_start + (location -> y << 10) + (location -> x << 1);
	*one_pixel_address = line_color;

}

//clear screen
void clear_screen(){
	for(int x = 0; x < 320; x++){
		for(int y = 0; y < 240; y++){
			xy_plot_pixel(x, y, 0x0);
		}
	}
}

//helper for draw line
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//draws a line using Bresenham's algorithm
void draw_line(int x0, int y0, int x1, int y1, short int colour){
	
	bool is_steep = abs(y1 - y0) > abs(x1 - x0);
	
 	if(is_steep){
		swap(&x0, &y0);
 		swap(&x1, &y1);
	}	
 	if(x0 > x1){
 		swap(&x0, &x1);
 		swap(&y0, &y1);
	}

 	int deltax = x1 - x0;
 	int deltay = abs(y1 - y0);
 	int error = -(deltax / 2);
	int y_step = 0;
 	int y = y0;
 	if(y0 < y1){
		y_step = 1;
	}else{
		y_step = -1;
	}

 	for(int x = x0; x <= x1; x++){
 		if(is_steep){
 			xy_plot_pixel(y, x, colour);
		}else{
 			xy_plot_pixel(x, y, colour);
		}
 		error = error + deltay;
 		if(error > 0){
 			y = y + y_step;
			error = error - deltax;
		}
	}
}

//draw player square, ONLY ODD VALUED SQUARESIZE
void draw_player_square(Square* square) {
    int half_side_length = (square->sideLength - 1) / 2;
    for (int x = square->position->x - half_side_length; x <= square->position->x + half_side_length; x++) {
        for (int y = square->position->y - half_side_length; y <= square->position->y + half_side_length; y++) {
            xy_plot_pixel(x, y, 0xFF00);
        }
    }
}

// Wait for screen to finish drawing
void wait_for_vsync() {
 	volatile int *pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;
    int bufferStatusBit;
    *(pixel_ctrl_ptr) = 1;                         // Write 1 into buffer register, causing a frame buffer swap.
    bufferStatusBit = *(pixel_ctrl_ptr + 3) & 1; // isolates S bit
    // implementing v-sync (waiting for S bit to go low)
    while (bufferStatusBit != 0) {
        bufferStatusBit = *(pixel_ctrl_ptr + 3) & 1;
    }
}
