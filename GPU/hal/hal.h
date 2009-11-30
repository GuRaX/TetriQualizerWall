/*******************************************************************************
*   Project:        TetriQualizerWall
*   Author:         Arne Schröder, 
*   Description:    Software for the GraphicController to send the messages to 
                    the RowController via I2C
*
*
*******************************************************************************/




#define MASK_BLUE   0xF0
#define MASK_PINK   0x0F

#define LEDs    16
#define COLORs  2

#define I2C_SPEED_400K  1
#define I2C_SPEED_100K  2

#ifndef F_CPU
#error "The preprocessor variable F_CPU needs to be defined!!"
#endif



extern void init_hardware( char i2c_speed, char uart_speed );


extern char set_row( char rowNr, char data[16] );  




