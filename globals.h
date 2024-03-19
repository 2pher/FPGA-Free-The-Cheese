#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "utilities.h"
#include "shapes.h"
#include "draw.h"


#define BOARD "DE1-SoC"

/* Memory */
#define DDR_BASE		        0x40000000
#define DDR_END				    0x7FFFFFFF
#define A9_ONCHIP_BASE		    0xFFFF0000
#define A9_ONCHIP_END		    0xFFFFFFFF
#define SDRAM_BASE			    0x00000000
#define SDRAM_END			    0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE	    0x08000000
#define FPGA_PIXEL_BUF_END	    0x0803FFFF
#define FPGA_CHAR_BASE		    0x09000000
#define FPGA_CHAR_END		    0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE		    	0xFF200000
#define LEDR_BASE		    	0xFF200000
#define HEX3_HEX0_BASE	    	0xFF200020
#define HEX5_HEX4_BASE	    	0xFF200030
#define SW_BASE			    	0xFF200040
#define KEY_BASE		    	0xFF200050
#define JP1_BASE	    		0xFF200060
#define JP2_BASE    			0xFF200070
#define PS2_BASE    			0xFF200100
#define PS2_DUAL_BASE		    0xFF200108
#define JTAG_UART_BASE		    0xFF201000
#define IrDA_BASE		    	0xFF201020
#define TIMER_BASE	    		0xFF202000
#define TIMER_2_BASE    		0xFF202020
#define AV_CONFIG_BASE  		0xFF203000
#define RGB_RESAMPLER_BASE      0xFF203010
#define PIXEL_BUF_CTRL_BASE 	0xFF203020
#define CHAR_BUF_CTRL_BASE  	0xFF203030
#define AUDIO_BASE		    	0xFF203040
#define VIDEO_IN_BASE		    0xFF203060
#define EDGE_DETECT_CTRL_BASE	0xFF203070
#define ADC_BASE			    0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE			    0xFFC04000
#define I2C1_BASE			    0xFFC05000
#define I2C2_BASE			    0xFFC06000
#define I2C3_BASE			    0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE		    	0xFFD0501C

/* Control registers to enable interrupts */
#define CTL0 0x1C000000
#define CTL3 0x1C00000C

extern int pixel_buffer_start;

extern bool LEFT, RIGHT, UP, DOWN;

/*****************************************************************************/
/* Macros for accessing the control registers.                               */
/*****************************************************************************/

#define NIOS2_READ_STATUS(dest) \
	do { dest = __builtin_rdctl(0); } while (0)

#define NIOS2_WRITE_STATUS(src) \
	do { __builtin_wrctl(0, src); } while (0)

#define NIOS2_READ_ESTATUS(dest) \
	do { dest = __builtin_rdctl(1); } while (0)

#define NIOS2_READ_BSTATUS(dest) \
	do { dest = __builtin_rdctl(2); } while (0)

#define NIOS2_READ_IENABLE(dest) \
	do { dest = __builtin_rdctl(3); } while (0)

#define NIOS2_WRITE_IENABLE(src) \
	do { __builtin_wrctl(3, src); } while (0)

#define NIOS2_READ_IPENDING(dest) \
	do { dest = __builtin_rdctl(4); } while (0)

#define NIOS2_READ_CPUID(dest) \
	do { dest = __builtin_rdctl(5); } while (0)



// Global variables
extern voltatile char byte1, byte2, byte3;

#endif