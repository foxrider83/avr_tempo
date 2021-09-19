/* 
 * File:   avr_tempo.h
 * Author: tux
 *
 * Created on 19 septembre 2021, 08:47
 */

void setup(void);
void start_tim1(void);
void stop_tim1(void);
void vibe(void);

#define COUNTER_START 0x6000
#define COUNTER_COMP 0x9896
#if defined(dbg)
    #define DELAY   3
#else
    #define DELAY   48  // 8 minutes
#endif

//Heart Beat LED
#define DDRHB  DDRD
#define PORTHB  PORTD
#define LEDHB   PORTD3

#define DDRVIB  DDRD
#define PORTVIB PORTD
#define VIBE    PORTD5
