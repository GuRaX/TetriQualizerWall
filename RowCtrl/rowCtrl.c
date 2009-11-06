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

#define LEDs   16
#define COLORs  2

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
volatile const uint16_t dimmLevel[16] = {0x0001, 0x1000, 0x2000, 0x3000,
                                         0x4000, 0x5000, 0x6000, 0x7000,
                                         0x8000, 0x9000, 0xA000, 0xB000,
                                         0xC000, 0xD000, 0xE000, 0xFFF0};

/* define Pins to select Color */
volatile struct t_port COLOR[2] = { {&DDRC, &PORTC, PC0}, {&DDRC, &PORTC, PC1} };

/* Status for each LED,  Bit 0-3 = BLUE  4-7 = PINK*/
volatile uint8_t LedStatus[16] = {  0xFF, 0x0F, 0xF0, 0xFF, 
                                    0xFF, 0x0F, 0xF0, 0xFF,
                                    0xFF, 0x0F, 0xF0, 0xFF,
                                    0xFF, 0x0F, 0xF0, 0xFF };

/*  */
volatile uint8_t currLED = 0;



void configurePins(void);
void wait_sec(void);

void TEST_Blink(char count);
void TEST_Run(char count);



int main(void) {
    configurePins();

    TEST_Blink(2);
    TEST_Run(2);  
    TEST_Blink(2);  

    OCR1A = 0x0000;
    OCR1B = 0x0000;
    TCCR1A = 0x00;
    TCCR1B = (1<<CS10);  /* no prescaler; normal operating mode */
    TIMSK = (1<<OCIE1A) | (1<<OCIE1B) | (1<<TOIE1); /* Enable both compare and the overflow interrupt */
    sei();

    while(1) {
    }

}



/******************************************************************************/



/* Interrupt to controll dimm level of the first(BLUE) color */
ISR(TIMER1_COMPA_vect) {
    *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
}



/* Interrupt to controll dimm level of the second(PINK) color */
ISR(TIMER1_COMPB_vect) {
    *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);
}


/* Interrupt to reset the old ports and select the new ports */
ISR(TIMER1_OVF_vect) {
    *LED[currLED].PORT &=~ (1<<LED[currLED].Pin);
    *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
    *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);

    currLED++;
    if (currLED >= LEDs) currLED = 0;

    OCR1A = dimmLevel[(LedStatus[currLED] & 0x0F)];
    OCR1B = dimmLevel[((LedStatus[currLED]>>4) & 0x0F)];

    *COLOR[BLUE].PORT |= (1<<COLOR[BLUE].Pin);
    *COLOR[PINK].PORT |= (1<<COLOR[PINK].Pin);
    *LED[currLED].PORT |= (1<<LED[currLED].Pin);
}



void configurePins(void) {
    char i;
    /* set all LED pins to output */
    for (i=0;i<LEDs;i++) {
        *LED[i].DDR |= (1<< LED[i].Pin);
        *LED[i].PORT &=~ (1<< LED[i].Pin);
    }
    /* set Color pins to output */
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
    for (i=0;i<LEDs;i++) {
        *LED[i].PORT |= (1<< LED[i].Pin);
    }

    while(count != 0) {
        *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
        *COLOR[PINK].PORT |= (1<<COLOR[PINK].Pin);
        _delay_ms(500);
        *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);
        *COLOR[BLUE].PORT |= (1<<COLOR[BLUE].Pin);
        _delay_ms(500);
        count--;
    }

    *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
    *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);

    for (i=0;i<LEDs;i++) {
        *LED[i].PORT &=~ (1<< LED[i].Pin);
    }
}



void TEST_Run(char count) {
    char i;
    while(count != 0) {
        *COLOR[PINK].PORT |= (1<<COLOR[PINK].Pin);
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT |= (1<< LED[i].Pin);
            if (i==0) {
                *LED[LEDs-1].PORT &=~ (1<< LED[LEDs-1].Pin);
            } else {
                *LED[i-1].PORT &=~ (1<< LED[i-1].Pin);
            }
            _delay_ms(200);
        }
        
        *COLOR[PINK].PORT &=~ (1<<COLOR[PINK].Pin);    
        *COLOR[BLUE].PORT |= (1<<COLOR[BLUE].Pin);
    
        for (i=0;i<LEDs;i++) {
            *LED[i].PORT |= (1<< LED[i].Pin);
            if (i==0) {
                *LED[LEDs-1].PORT &=~ (1<< LED[LEDs-1].Pin);
            } else {
                *LED[i-1].PORT &=~ (1<< LED[i-1].Pin);
            }
            _delay_ms(200);
        }
        *COLOR[BLUE].PORT &=~ (1<<COLOR[BLUE].Pin);
        count--;
    }
}



