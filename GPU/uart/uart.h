#ifndef __UART_H__
#define __UART_H__

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../GPU/config.h"

#ifndef F_CPU
#error "F_CPU need to be defined!!"
#endif




#define CALC_BAUDRATE(baudRate,cpu) ((cpu)/((baudRate)*16l)-1)


extern void uart_init(uint16_t baudrate);

extern void uart_putc(char c);

extern void uart_puts(char* str);


#endif /* __UART_H__ */
