#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serialCom.h"
#include "util.h"

#define DELAY 20
#define RESISTOR 1500

int __attribute__((OS_main)) main (void)
{
  USART_init();
  ADMUX |= (1<<REFS0);
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

  while (1) {
    ADCSRA |= (1<<ADSC);
    while(!(ADCSRA & (1<<ADIF))){}
    int32_t rawData = ADC;
    int32_t longData = rawData * 5;
    int32_t tempResistance = longData * RESISTOR / ( 5000 - longData );
    int32_t temp = -96.4651020295015F + 0.116981977579288F * tempResistance;
    putDec(temp);
    _delay_ms(1000);
  }
}
