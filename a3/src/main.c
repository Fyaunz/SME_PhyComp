#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serialCom.h"
#include "util.h"
//#include <avr/iom328p.h>

volatile int16_t disStart = 0;
volatile int16_t disEnd = 0;
volatile uint64_t disbuffer[64];

ISR(TIMER1_CAPT_vect){

    uint64_t duration =  ICR1;
    duration = duration / 2;
    disbuffer[disEnd] = ((duration * 34.3F) / 2000) -5;
    disEnd = (disEnd + 1) & 63;
}

void ping(void){
    ICR1 &= 0x0000;
    PORTD |= (1<<PORTD3);
    _delay_us(10);
    PORTD &= ~(1<<PORTD3);
    TCNT1 &= 0x0000;
}

int __attribute__((OS_main)) main (void)
{
  USART_init();

  TCCR1B |= (1<<CS11);
  TIMSK1 |= (1<<ICIE1);

  DDRD = (1<<DDD3);
  ICR1 &= 0x0000;

  while(1){

    ping();

  /*
    uint64_t duration = ICR1;
    duration = duration / 2;
    //uint64_t distance = (duration * 100) / 5882 ;
    uint64_t distance = ((duration * 34.3F) / 2000);
    if(distance > 4 && distance < 400){
      putDec(distance);
    } */

    if(disStart != disEnd){
      uint32_t distance = disbuffer[disStart];
      disStart = (disStart + 1) & 63;
      if(distance > 2 && distance < 400){
        putDec(distance);
      }
    } 

    ICR1 &= 0x0000;

    _delay_ms(500);

    
  }
}
