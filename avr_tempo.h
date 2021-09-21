/* 
 * File:   avr_tempo.h
 * Author: tux
 *
 * Created on 19 septembre 2021, 08:47
 */

void setup(void);
void vibe(void);
void vibe2(void);

#define COUNTER_START 0x6000
#define COUNTER_COMP 0x9896
#if defined(dbg)
    #define DELAY   2
#else
    #define DELAY   48  // 8 minutes
#endif
#if defined(dbg)
    #define DELAYEND   5
#else
    #define DELAYEND   60  // 10 minutes
#endif


//Heart Beat LED
#define DDRHB  DDRD
#define PORTHB  PORTD
#define LEDHB   PORTD3

#define DDRVIB  DDRD
#define PORTVIB PORTD
#define VIBE    PORTD5

// MACRO
#define start_tim1  (TCCR1B |= (1<<CS12))
#define stop_tim1   (TCCR1B &= ~(1<<CS12))