#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serialCom.h"
#include "util.h"

#define DELAY 20

int __attribute__((OS_main)) main (void)
{
  int red = 128;
  int green = 0;
  int blue = 0;

  //USART_init(); // Initialisierung der seriellen Schnittstelle

  DDRB = (1<<DDB2)|(1<<DDB1);
  DDRD |= (1<<DDD3);


  TCCR2A = (1<<COM2A1)|(1<<COM2B1)|(1<<WGM20);
  TCCR2B |= (1<<CS22);

  TCCR1A = (1<<COM1A1)|(1<<COM1B1)| (1<<WGM10);
  TCCR1B = (1<<CS11)|(1<<CS10);

  OCR1B = red;
  OCR1A = blue;
  OCR2B = green;

  while(1) {
    do {
      OCR1B = red--;
      OCR2B = green++;
      _delay_ms(DELAY);
    }while (red != 0);

    do {
      OCR2B = green--;
      OCR1A = blue++;
      _delay_ms(DELAY);
    }while (green != 0);

      do {
      OCR1A = blue--;
      OCR1B = red++;
      _delay_ms(DELAY);
    }while (blue != 0);

  }
}
