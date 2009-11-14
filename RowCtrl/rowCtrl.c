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


//#define NONE    0
#define BLUE    0
#define PINK    1
//#define ALL     3

#define LEDs    16
#define COLORs  2

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

volatile uint8_t LedStatus[16] = {  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
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

    *COLOR[PINK].PORT |= (1<<COLOR[PINK].Pin);
    
    TCNT0 = TIMER_START;
    TIMSK |= (1<<TOIE0);
    TCCR0 |= (1<<CS01) | (1<<CS00);

    sei();

    while(1) {
        char i;
        _delay_ms(200);
        for (i=0; i<LEDs; i++) {
            if (++LedStatus[i] >= 15) LedStatus[i] = 0;
        }
    }

}



/******************************************************************************/


ISR(TIMER0_OVF_vect) {
    static cLed;
    static cCol;
    
    *LED[cLed].PORT &=~ (1<<LED[cLed].Pin);
    if (++cLed >= LEDs) {
        cLed = 0;
    }
    if (dimmLevel[ LedStatus[cLed] ] != 0) *LED[cLed].PORT |= (1<<LED[cLed].Pin);
    TCNT0 = TIMER_START + dimmLevel[ LedStatus[cLed] ];
}

/* Interrupt to controll dimm level of the first(BLUE) color */
/*
ISR(TIMER1_COMPA_vect) {
    *LED[currLED].PORT &=~ (1<<LED[currLED].Pin);

    currLED++;
    if (currLED >= LEDs) currLED = 0;

    OCR1A = dimmLevel[(LedStatus[currLED] & 0x0F)];

    if ( OCR1A != 0 )*LED[currLED].PORT |= (1<<LED[currLED].Pin);
    TCNT1 = 0x0000;

}
*/


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



