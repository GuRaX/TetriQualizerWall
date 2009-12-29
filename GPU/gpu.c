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
    init_hardware(I2C_SPEED_400K, 38400UL);
//    char testrow[LEDs] = {0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,
//                          0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10};

//    char testrow[LEDs] = {'a','b','c','d','e','f','g','h',
//                          'i','j','k','l','m','n','o','p'};

//  char testrow[LEDs] = {'a','a','a','a','a','a','a','a',
//                        'd','d','d','d','d','d','d','d'};

    char display[24][16];
    char tmp[24];
    uint8_t i,j;
    char newData = 0x00;
    char ctrl_data[2] = {0,0};
    char ctrl_data_old[2] = {0,0};
   
    for (i=0;i<24;i++) {
        for (j=0;j<16;j++) {
            display[i][j] = 0x00;
        }
    }

    display[0][0] = 0x0F;
    sei();
    while(1) 
    {
/* just send example row for the display over the connection * /
        for (i=0; i<ROWs; i++) {
            set_row( i+1, testrow);
        }
        _delay_ms(500);*/

/* check controller for data and send them via UART */
        get_controller_data(ctrl_data);
/*        if ( (ctrl_data[0] != ctrl_data_old[0]) || 
             (ctrl_data[1] != ctrl_data_old[1])    ) { */
/* second controller currently not connected to the board */
        if ( ctrl_data[0] != ctrl_data_old[0] ) { 
/*            uart_putc(0xAA);
            uart_putc(ctrl_data[0]);
/ *            uart_putc(ctrl_data[1]);*/

            /* Controller Up */
            if ( (ctrl_data[0] & NES_UP) != 0 ) {
                for (i=0;i<16;i++) {
                    tmp[i] = display[0][i];
                }
                for (i=1;i<24;i++) {
                    for (j=0;j<16;j++) {
                        display[i-1][j] = display[i][j];
                    }
                }
                for (i=0;i<16;i++) {
                    display[23][i] = tmp[i];
                }
                newData = 1;
            }
            
            /* Controller Down */
            if ( (ctrl_data[0] & NES_DOWN) != 0 ) {
                for (i=0;i<16;i++) {
                    tmp[i] = display[23][i];
                }
                for (i=23;i>0;i--) {
                    for (j=0;j<16;j++) {
                        display[i][j] = display[i-1][j];
                    }
                }
                for (i=0;i<16;i++) {
                    display[0][i] = tmp[i];
                }
                newData = 1;
            }
            
            /* Conrtoller Left */
            if ( (ctrl_data[0] & NES_LEFT) != 0 ) {
                for (i=0;i<24;i++) {
                    tmp[1] = display[i][0];
                    for (j=1;j<16;j++) {
                        display[i][j-1] = display[i][j];
                    }
                    display[i][15] = tmp[1];
                }
                newData = 1;
            }

            /* Conrtoller Right */
            if ( (ctrl_data[0] & NES_RIGHT) != 0 ) {
                for (i=0;i<24;i++) {
                    tmp[1] = display[i][15];
                    for (j=15;j>0;j--) {
                        display[i][j] = display[i][j-1];
                    }
                    display[i][0] = tmp[1];
                }
                newData = 1;
            }
            /* Put new Display data */
            if (newData) {
                newData = 0;
                for (i=0; i<ROWs; i++) {
                    set_row( i, display[i]);
                }
            }

            ctrl_data_old[0] = ctrl_data[0];
            ctrl_data_old[1] = ctrl_data[1];

        }

    }

}







