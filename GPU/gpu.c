/*******************************************************************************
*   Project:        TetriQualizerWall
*   Author:         Arne Schröder, 
*   Description:    Software for the GraphicController to send the messages to 
                    the RowController via I2C
*
*
*******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hal/hal.h"




int main(void) {
    init_hardware(100000L, 9600L);

}







