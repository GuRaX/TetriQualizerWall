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

volatile uint8_t TIMER_START[COLORs] = {150,100}; //155

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
/*volatile const uint8_t dimmLevel[16] = { 99, 97, 91, 85, 78, 72, 66, 50,
                                         44, 37, 31, 25, 18, 12,  6,  0 };*/
volatile const uint8_t dimmLevel[4] = { 0, 99, 50, 6 };
/* define Pins to select Color */
volatile struct t_port COLOR[2] = { {&DDRC, &PORTC, PC0}, {&DDRC, &PORTC, PC1} };

volatile uint8_t LedStatus[16] = { 0x03, 0x03, 0x12, 0x12, 0x21, 0x21, 0x30, 0x30,
                                   0x30, 0x30, 0x21, 0x21, 0x12, 0x12, 0x03, 0x03};
volatile uint8_t currLED = 0;



void configurePins(void);
void wait_sec(void);

void TEST_Blink(char count);
void TEST_Run(char count);



int main(void) {
    configurePins();

    //TEST_Blink(4);
    //TEST_Run(2);  
    //TEST_Blink(2);  

    TCNT0 = TIMER_START;
    TIMSK |= (1<<TOIE0);
    TCCR0 |= (1<<CS01) | (1<<CS00);

    sei();

    while(1) {

        char i, tmp;
        _delay_ms(50);
        tmp = LedStatus[0];
        for (i=0; i<LEDs-1; i++) {
            LedStatus[i] = LedStatus[i+1];
        }
        LedStatus[LEDs-1] = tmp;
        /*
        for (i=0; i<LEDs; i++) {
            if (i != LEDs-1) {
                LedStatus[i] = (LedStatus[i] & 0x0F) | (LedStatus[i+1] & 0x0F);
            } else {
                LedStatus[i] = (LedStatus[i] & 0x0F) | (tmp & 0x0F);
            }
        }
        tmp = LedStatus[LEDs-1];
        for (i=LEDs-1; i>0; i--) {
            if (i != 0) {
                LedStatus[i] = (LedStatus[i] & 0xF0) | (LedStatus[i-1] & 0xF0);
            } else {
                LedStatus[i] = (LedStatus[i] & 0xF0) | (tmp & 0xF0);
            }
        }
        */

    }

}



/******************************************************************************/



ISR(TIMER0_OVF_vect) {
    static uint8_t cColor;
    static uint8_t shift;
    static uint8_t step;
    uint8_t i;
    
    
    if (step == 0x00) {
        if (cColor == BLUE) {
            cColor = PINK;
            shift = 4;
        } else {
            cColor = BLUE;
            shift = 0;
        }
        /* start - all leds with dimmlevel to ON */
        for (i=0; i < COLs; i++) {
            if (((LedStatus[i]>>shift) & 0x0F) != 0x00) {
                *LED[i].PORT |= (1<<LED[i].Pin);
            }
        }
        *COLOR[cColor].PORT |= (1<<COLOR[cColor].Pin);
        step = 0x01;
        TCNT0 = TIMER_START[cColor];
    }
    
    if (step == 0x01) {
        /* all leds with dimmlevel 1 to OFF */
        for (i=0; i < COLs; i++) {
            if (((LedStatus[i]>>shift) & 0x0F) == 0x01) {
                *LED[i].PORT &=~ (1<<LED[i].Pin);
            }
        }
        step = 0x02;
        TCNT0 = TIMER_START[cColor];
    }
    
    if (step == 0x02) {
        /* all leds with dimmlevel 2 to OFF */
        for (i=0; i < COLs; i++) {
            if (((LedStatus[i]>>shift) & 0x0F) == 0x02) {
                *LED[i].PORT &=~ (1<<LED[i].Pin);
            }
        }
        step = 0x03;
        TCNT0 = TIMER_START[cColor];
    }
    
    if (step == 0x03) {
        /* all leds with dimmlevel 3 to OFF */
        for (i=0; i < COLs; i++) {
            if (((LedStatus[i]>>shift) & 0x0F) == 0x03) {
                *LED[i].PORT &=~ (1<<LED[i].Pin);
            }
        }
        *COLOR[cColor].PORT &=~ (1<<COLOR[cColor].Pin);
        step = 0x00;
        TCNT0 = TIMER_START[cColor];
    }
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
        _delay_ms(100);
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



