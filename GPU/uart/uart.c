#include "uart.h"



extern void uart_init(uint16_t baudrate)
{
    UCSR1B |= (1<<TXEN1);             // UART TX einschalten
    UCSR1C |= (1<<UCSZ10)|(1<<UCSZ11); // Asynchron 8N1
    
    UBRR1H = (baudrate>>8) & 0xFF;
    UBRR1L = baudrate & 0xFF;
}



extern void uart_putc(char c)
{
    while (!(UCSR1A & (1<<UDRE1))) { 
    }
    UDR1 = c;
}



extern void uart_puts(char* str)
{
    while (*str) {
        uart_putc((char)*str);
        str++;
    }
}
