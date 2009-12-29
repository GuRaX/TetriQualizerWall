#ifndef __HAL_H__
#define __HAL_H__
/*******************************************************************************
 *  Project:        TetriQualizerWall
 *  Author:         Arne Schröder, 
 *  Description:    Software for the GraphicController to send the messages to 
 *                  the RowController via I2C
 *
 *
 ******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../i2c/i2cmaster.h"
#include "../uart/uart.h"
#include "../GPU/config.h"


#define I2C_SPEED_400K  1
#define I2C_SPEED_100K  2

#define DEBUG 1

#ifndef F_CPU
#error "The preprocessor variable F_CPU needs to be defined!!"
#endif

#ifndef DEBUG
#warning "### DEBUG variable not defined!! debugging disabled (DEBUG = 0)"
#define DEBUG 0
#endif

#if DEBUG == 0
#define DBG_I2C   1
#warning "### Debug disabled !!"
#endif
#if DEBUG == 1
#define DBG_RS232   1
#warning "### Debug over RS232 Enabled / I2C Disabled"
#endif
#if DEBUG == 2
#define DBG_RS232   1
#define DBG_I2C     1
#warning "### Debug over RS232 Enabled and I2C Enabled"
#endif

#define CTRL_DDR    DDRA
#define CTRL_PORT   PORTA
#define CTRL_PIN    PINA
#define CTRL_CLOCK  PA2
#define CTRL_LATCH  PA1
#define CTRL1_DATA  PA0
#define CTRL2_DATA  PA3

extern void init_hardware( char i2c_speed, uint16_t uart_speed );


extern char set_row( char rowNr, char data[16] );  


extern void get_controller_data( char data[2] );


#endif // __HAL_H__
