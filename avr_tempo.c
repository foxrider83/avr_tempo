/*
 * File:   avr_tempo.c
 * Author: JSO
 * Created on 15 septembre 2021, 13:42
 * Delay of 8 minutes and after activate vibration
 * Use Timer1 to count time with interrupt.
 * Two solution for counting :
 * - count from 0 to OCR1A
 * - preset TCNT1 and count until overflow (easiest)
 */
#define F_CPU 1000000UL
//#define dbg

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
//#include <xc.h>
#include <util/delay.h>
#include "avr_tempo.h"

#define ovf
//Status byte x x x vib2 vib1 delaye delay ovfb
uint8_t status;
#define BVIB2   4
#define BVIB1   3
#define BDELAYE 2
#define BDELAY  1
#define BOVF    0

int main(void) {
    volatile uint8_t time_counter = 0;
    status = 0;
    setup();
    vibe2();
    sei();
    //Start Timer1
    start_tim1;
    while (1) {
        if (status & (1 << BOVF)){
            //Timer 1 overflow occurs
            stop_tim1;
            TCNT1 = COUNTER_START;
            TIFR1 &= ~(TOV1);
            PORTHB ^= (1 << LEDHB);
            time_counter ++;
            status &= ~(1 << BOVF);     // reset overflow status
            start_tim1;           
        }
        if (time_counter >= DELAY){
            if ((status & (1 << BVIB1)) == 0){
                status |= (1 << BDELAY);    // Delay reached            
            }
        }
        if (time_counter >= DELAYEND){
            status |= (1 << BDELAYE);
        }
        if (status & (1 << BDELAY)){
            //First delay reached
            vibe();
            status |= (1 << BVIB1);     // first vibration occured
            status &= ~(1 << BDELAY);   // reset status delay overflow.
        }
        if (status & (1 << BDELAYE)){
            stop_tim1;
            vibe2();
            //Last delay reached
            time_counter = 0;
            status &= ~(1<< BDELAYE);   // reset status delay end overflow
            // Sleep mode until reset the microcontroller. POWER DOWN
            SMCR |= (1 << SM1);
        }
    }
}

/*
 * Configuration of the microcontroller
 */
void setup(void){
    //Configure LED Heart Beat
    DDRHB |= (1 << LEDHB);
    DDRVIB |= (1 << VIBE);
    //Timer1 clock configuration Fcpu/256
    //TCCR1B |= (1<<CS12);
    TCCR1B &= ~((1<<CS10) | (1<<CS11));
    #if defined(ovf)
    //Set 
    TCNT1 = COUNTER_START;
    TIMSK1 |=(1<<TOIE1);
    #else
    OCR1A = COUNTER_COMP;
    TIMSK1 |= (1<<OCIE1A);
    #endif
    status = 0;
    return;
}

/*
 *    Activate the vibrator
 */
void vibe(void){
    PORTVIB |= (1 << VIBE);
    _delay_ms(800);
    PORTVIB ^= (1 << VIBE);
    _delay_ms(500);
    PORTVIB ^= (1 << VIBE);
    _delay_ms(500);
    PORTVIB &= ~(1 << VIBE);
}

/*
 *    Activate the vibrator for time up
 */
void vibe2(void){
    PORTVIB |= (1 << VIBE);
    _delay_ms(800);
    PORTVIB &= ~(1 << VIBE);
    _delay_ms(500);
    PORTVIB |= (1 << VIBE);
    _delay_ms(800);
    PORTVIB &= ~(1 << VIBE);
    _delay_ms(500);
    PORTVIB |= (1 << VIBE);
    _delay_ms(800);
    PORTVIB &= ~(1 << VIBE);
}
/*
 * Interrupts
 */
#if defined(ovf)
ISR(TIMER1_OVF_vect){
    //volatile uint8_t sreg;
    //sreg = SREG;
    cli();
    status |= (1 << BOVF);
    sei();
    //SREG = sreg;
}
#else
ISR(TIMER1_COMPA_vect){
    uint8_t sreg;
    cli();
    sreg = SREG;
    //Reset Timer
    TCNT1 = 0x00;
    SREG = sreg;
    sei(); 
}
#endif