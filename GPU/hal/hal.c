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
#include "../i2c/twimaster.c"
#include "../uart/uart.h"


void init_hardware( char i2c_speed, char uart_speed ) 
{
    i2c_init(i2c_speed);
    
}


char set_row( char rowNr, char data[16] )  
{
    char err = 0;
    


    return err;
}







