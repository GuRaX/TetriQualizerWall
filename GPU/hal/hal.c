/*******************************************************************************
 *   Project:        TetriQualizerWall 
 *                       - GPU 
 *                           - Hardware Abstraction Layer
 *   Author:         Arne Schröder, 
 *   Description:    Software for the GraphicController to send the messages to 
 *                   the RowController via I2C
 *
 *
 ******************************************************************************/

#include "hal.h"



/*******************************************************************************
 * @brief initialize the controller io pins and set the configuration bits 
 *        for the selected transmission speed
 * 
 * @param i2c_speed    Defines the speed of the I2C bus normaly 100kHz or 400kHz
 * @param uart_speed   Defines the speed of the Uuart1 serial port (bsp 9600Hz)
 * 
 * @todo    Check relation between I2C and Uart1 speed !!
 ******************************************************************************/
void init_hardware( char i2c_speed, uint16_t uart_speed ) 
{
    /* initialize i2c for display communication */
#ifdef DBG_I2C
    i2c_init(i2c_speed);
#endif
    
    /* initialize uart for debugging */
#ifdef DBG_RS232
    uart_init( CALC_BAUDRATE(uart_speed ,F_CPU) );
#endif

    /* initialize IO Pins for NES Controllers */
    /* set Port direction */
    CTRL_DDR |= (1<<CTRL_CLOCK) | (1<<CTRL_LATCH);
    CTRL_DDR &=~ (1<<CTRL1_DATA) | (1<<CTRL2_DATA);
    /* set pullup/down resistors */
    CTRL_PORT |= (1<<CTRL_CLOCK) | (1<<CTRL_LATCH);
}


char set_row( char rowNr, char data[16] )  
{
    char err = 0;
    char i = 0;
    rowNr = (rowNr<<1) | I2C_WRITE;
#ifdef DBG_RS232
    uart_putc(0xAA);   // start byte
    uart_putc(rowNr);   // address byte
#endif
#ifdef DBG_I2C
    i2c_start(rowNr);
#endif
    for (i=0; i<LEDs; i++) 
    {
#ifdef DBG_RS232
        uart_putc(data[i]);
#endif
#ifdef DBG_I2C
        i2c_write(data[i]);
#endif
    }
    

    return err;
}


void get_controller_data( char data[2] ) {
    char i=0;
    data[0] = 0;
    data[1] = 0;
    CTRL_PORT &=~ (1<<CTRL_LATCH) | (1<<CTRL_CLOCK);
    CTRL_PORT |= (1<<CTRL_LATCH);
//    _delay_ns(250);
    _delay_ms(1);
    CTRL_PORT &=~ (1<<CTRL_LATCH);
//    _delay_ns(50);
    _delay_ms(1);
    for (i=0; i<8; i++) {
        /* if bit is 1 the button on the ctroller is NOT pressed */
        if (CTRL_PIN & (1<<CTRL1_DATA))  data[0] = (data[0]<<1);
            else data[0] = (data[0]<<1) | 1;
        if (CTRL_PIN & (1<<CTRL2_DATA))  data[1] = (data[1]<<1);
            else data[1] = (data[1]<<1) | 1;
        CTRL_PORT |= (1<<CTRL_CLOCK);
//        _delay_ns(250);
        _delay_ms(1);
        CTRL_PORT &=~ (1<<CTRL_CLOCK);
//        _delay_ns(50);
        _delay_ms(1);
    }
    
}




