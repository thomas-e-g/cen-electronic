#ifndef MECHANICAL_BOARD_1_H
#define MECHANICAL_BOARD_1_H

#include <p30fxxxx.h>

/** Define the I2C address used by motorBoard. */
#define MECHANICAL_BOARD_1_I2C_ADDRESS 	0x52

// SPECIFIC 2011

#define PIN_SWITCH_PLIER_OPEN_RIGHT		PORTBbits.RB10

#define PIN_SWITCH_PLIER_OPEN_LEFT		PORTBbits.RB11

#define PIN_SWITCH_PLIER_CLOSE_RIGHT	PORTBbits.RB9

#define PIN_SWITCH_PLIER_CLOSE_LEFT		PORTBbits.RB8

#endif