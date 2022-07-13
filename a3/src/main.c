#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "serialCom.h"
#include "util.h"
//#include <avr/iom328p.h>

int __attribute__((OS_main)) main (void)
{

  USART_init();
  uint8_t data = getChar();

  while (data != 'a');
  read_all();
  
}
