/*
 * File:   avr_tempo.c
 * Author: JSO
 * Created on 15 septembre 2021, 13:42
 * Delay of 8 minutes and after activate vibration
 * Use Timer0 to count time with ISR and Sleep mode
 */
#include <avr/io.h>
#include <xc.h>
#include <util/delay.h>



void setup(void){
    //Set PortB5 to output and set to Off
    DDRB |= (1<<DDB5);
    PORTB &= ~(1<<PORTB5);
    return;
}

int main(void) {
    PORTB = 0xFF;
    setup();
    /* Replace with your application code */
    //PORTC = 1;
    while (1) {
        PORTB ^= (1 << PINB5);
        //_delay_ms(100);
        PORTC ^= (1 << PINC0);
    }
}

void ISR(TIMER0_OVF_vect){
    
}