// written by Ethanak
// https://forbot.pl/forum/topic/16494-tresowanie-odkurzacza-czyli-wirtualna-sciana-na-attiny13/

#include <Arduino.h>

#ifndef F_CPU
#define F_CPU 9600000UL
#endif

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <util/delay.h>

ISR(WDT_vect){
}

#define pon do { \
        TCCR0A |= (1<<WGM01) | (1<<COM0A0);\
        TCCR0B |= (1<<CS00); \
      } while(0)
 
#define poff do { \
        TCCR0A &= ~((1<<WGM01) | (1<<COM0A0)); \
        TCCR0B &= ~(1<<CS00); \
        PORTB &= ~(1 << PB0);\
      } while (0)
 
int main(void){
    char i, n;
    DDRB |= (1 << PB0) | (1 << PB1);
    PORTB |= (1 << PB1);
    OCR0A = 125;
    
    // watchdog
    
    MCUSR &= ~(1<<WDRF);
    WDTCR |= (1 << WDCE) | (1 << WDE);
    WDTCR = 1 | (1 << WDCE);
    WDTCR |= (1 << WDTIE);
    
    sei();

    for (n=0;; n= (n + 1) & 7) {
        if (!n) PORTB |= (1 << PB1);
        for (i=0; i<3; i++) {
            pon;
            _delay_us(500);
            poff;
            if (i<2) _delay_us(7500);
        }
        if (!n) PORTB &= ~(1 << PB1);
        for (i=0; i<5; i++) {
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();
        }
    }  
}