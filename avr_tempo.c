/*
 * File:   avr_tempo.c
 * Author: JSO
 * Created on 15 septembre 2021, 13:42
 * Delay of 8 minutes and after activate vibration
 * Use Timer0 to count time with interrupt and Sleep mode.
 * Two solution for counting :
 * - count from 0 to OCR1A
 * - preset TCNT1 and count until overflow
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
//#include <xc.h>
#include <util/delay.h>


#define F_CPU 1000000UL
#define ovf
#define COUNTER_START 0x6769
#define COUNTER_COMP 0x9896
void setup(void);
void start_tim1(void);
void stop_tim1(void);

int main(void) {
    PORTB = 0xFF;
    setup();
    //Start Timer1
    
    while (1) {
        PORTB ^= (1 << PINB5);
        //_delay_ms(100);
        PORTC ^= (1 << PINC0);
    }
}

void setup(void){
    //Set PortB5 to output and set to Off
    DDRB |= (1<<DDB5);
    PORTB &= ~(1<<PORTB5);
    //Timer1 clock configuration Fcpu/256
    //TCCR1B |= (1<<CS12);
    TCCR1B &= ~((1<<CS10) | (1<<CS10));
    #if defined(ovf)
    //Set 
    TCNT1 = COUNTER_START;
    TIMSK1 |=(1<<TOIE1);
    #else
    OCR1A = COUNTER_COMP;
    TIMSK1 |= (1<<OCIE1A);
    #endif  
    return;
}

void start_tim1(void){
    TCCR1B |= (1<<CS12);
}

void stop_tim1(void){
    TCCR1B &= ~(1<<CS12);
}

#if defined(ovf)
ISR(TIMER0_COMPA_vect){
    uint8_t sreg;
    cli();
    sreg = SREG;
    //Reset Timer
    TCNT1 = 0x00;
    SREG = sreg;
    sei();    
}
#else
ISR(TIMER0_OVF_vect){
    cli();
    TCNT1 = COUNTER_START;
    sei();
}
#endif