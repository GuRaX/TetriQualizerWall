/*******************************************************************************
*   Project:        TetriQualizerWall
*   Author:         Arne Schröder, 
*   Description:    Software to Controll one row of the Display
*
*
*******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../GPU/config.h"

/*
//#define NONE    0
#define BLUE   0
#define PINK   1
//#define ALL     3

#define LEDs    16
#define COLORs  2
*/

#define TIMER_START 155

struct t_port {
    uint8_t *DDR;
    uint8_t *PORT;
    uint8_t Pin;
};

/* define Pins for each LED */
volatile struct t_port LED[16] = { {&DDRD, &PORTD, PD0}, 
                                   {&DDRD, &PORTD, PD1}, 
                                   {&DDRD, &PORTD, PD2}, 
                                   {&DDRD, &PORTD, PD3},
                                   {&DDRD, &PORTD, PD4},
                                   {&DDRD, &PORTD, PD5},
                                   {&DDRD, &PORTD, PD6},
                                   {&DDRD, &PORTD, PD7},
                                   {&DDRB, &PORTB, PB0},
                                   {&DDRB, &PORTB, PB1},
                                   {&DDRB, &PORTB, PB2},
                                   {&DDRB, &PORTB, PB3},
                                   {&DDRB, &PORTB, PB4},
                                   {&DDRB, &PORTB, PB5},
                                   {&DDRC, &PORTC, PC2},
                                   {&DDRC, &PORTC, PC3} };

/* timer values for the different dimm levels */
volatile const uint8_t dimmLevel[16] = { 99, 97, 91, 85, 78, 72, 66, 50,
                                         44, 37, 31, 25, 18, 12,  6,  0 };

/* define Pins to select Color */
volatile struct t_port COLOR[2] = { {&DDRC, &PORTC, PC0}, {&DDRC, &PORTC, PC1} };

volatile uint8_t LedStatus[16] = {  0xF0,0xE1,0xD2,0xC3,0xB4,0xA5,0x96,0x87,0x78,0x69,0x5A,0x4B,0x3C,0x2D,0x1E,0x0F };
volatile uint8_t currLED = 0;



void configurePins(void);
void wait_sec(void);

void TEST_Blink(char count);
void TEST_Run(char count);



int main(void) {
    configurePins();

    TEST_Blink(4);
    //TEST_Run(2);  
    //TEST_Blink(2);  

    TCNT0 = TIMER_START;
    TIMSK |= (1<<TOIE0);
    TCCR0 |= (1<<CS01) | (1<<CS00);

    sei();

    while(1) {
/*
        char i;
        _delay_ms(100);
        for (i=0; i<LEDs; i++) {
            
            if (LedStatus[i] >= 15) LedStatus[i] = 0;
        }
*/
    }

}



/******************************************************************************/


ISR(TIMER0_OVF_vect) {
    static char cLed;
    static char cCol;
    char dlvl;
    
    *LED[cLed].PORT &=~ (1<<LED[cLed].Pin);
    cLed++;
    if (cLed >= LEDs) {
        cLed = 0;
        if (cCol == 0) {
            cCol = 1;
            *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
            *COLOR[PINK].PORT |= (1<<COLOR[PINK].Pin);
        } else {
            cCol = 0;
            *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);
            *COLOR[BLUE].PORT |= (1<<COLOR[BLUE].Pin);
        }
    }

    if (cCol == 0) {
        dlvl = dimmLevel[ (LedStatus[cLed] & 0x0F) ];
    } else {
        dlvl = dimmLevel[ (LedStatus[cLed] & 0xF0)>>4 ];
    }

    if (dlvl != 0) 
        *LED[cLed].PORT |= (1<<LED[cLed].Pin);

    TCNT0 = TIMER_START + dlvl;
}



void configurePins(void) {
    char i;
    /* set all LED pins to output */
    for (i=0;i<LEDs;i++) {
        *LED[i].DDR |= (1<< LED[i].Pin);
        *LED[i].PORT &=~ (1<< LED[i].Pin);
    }
    /* set all COLOR pins to output */
    for (i=0;i<COLORs;i++) {
        *COLOR[i].DDR |= (1<< COLOR[i].Pin);
        *COLOR[i].PORT &=~ (1<< COLOR[i].Pin);
    }
}



void wait_sec(void){
    char i=0;    
    for (i=0; i<10; i++) {
        _delay_ms(10);
    }
}



void TEST_Blink(char count) {
    char i;
    *COLOR[BLUE].PORT |= (1<<COLOR[BLUE].Pin);
    while(count) {
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT |= (1<< LED[i].Pin);
        }
        _delay_ms(100);
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT &=~ (1<<LED[i].Pin);
        }
        _delay_ms(100);
        count--;
    }
    *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
    *COLOR[PINK].PORT |= (1<<COLOR[PINK].Pin);
    while(count) {
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT |= (1<< LED[i].Pin);
        }
        _delay_ms(100);
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT &=~ (1<<LED[i].Pin);
        }
        _delay_ms(100);
        count--;
    }
    *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);

}



void TEST_Run(char count) {
    char i;
    *COLOR[BLUE].PORT |= (1<<COLOR[BLUE].Pin);
    while(count != 0) {
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT |= (1<< LED[i].Pin);
            if (i==0) {
                *LED[LEDs-1].PORT &=~ (1<< LED[LEDs-1].Pin);
            } else {
                *LED[i-1].PORT &=~ (1<< LED[i-1].Pin);
            }
            _delay_ms(200);
        }
        count--;
    }
    *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
}



