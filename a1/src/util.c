#include <avr/io.h>
#include <util/delay.h>
#define BLINK_DELAY_MS 200

void blink(void){
  DDRD |= (1<<DDD4);
    // turn LED on
    PORTD |= (1 << PORTD4);
    _delay_ms(BLINK_DELAY_MS);

    // turn LED off
    PORTD &= ~ (1 << PORTD4);
    _delay_ms(BLINK_DELAY_MS);
}
