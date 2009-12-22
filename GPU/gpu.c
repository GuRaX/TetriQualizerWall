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

#include "config.h"
#include "hal/hal.h"



int main(void) {
    init_hardware(I2C_SPEED_400K, 9600);
//    char testrow[LEDs] = {0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,
//                          0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10};

//    char testrow[LEDs] = {'a','b','c','d','e','f','g','h',
//                          'i','j','k','l','m','n','o','p'};

    char testrow[LEDs] = {'a','a','a','a','a','a','a','a',
                          'd','d','d','d','d','d','d','d'};

    char i;
    char ctrl_data[2] = {0,0};
    char ctrl_data_old[2] = {0,0};
    
    sei();
    while(1) 
    {
/* just send example row for the display over the connection * /
        for (i=0; i<ROWs; i++) {
            set_row( i+1, testrow);
        }
        _delay_ms(500);
*/
/* check controller for data and send them via UART */
        get_controller_data(ctrl_data);
/*        if ( (ctrl_data[0] != ctrl_data_old[0]) || 
             (ctrl_data[1] != ctrl_data_old[1])    ) { */
/* second controller currently not connected to the board */
        if ( ctrl_data[0] != ctrl_data_old[0] ) { 
            uart_putc(0xAA);
            uart_putc(ctrl_data[0]);
/*            uart_putc(ctrl_data[1]);*/
            ctrl_data_old[0] = ctrl_data[0];
            ctrl_data_old[1] = ctrl_data[1];
        }

    }

}







