#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "serialCom.h"
#include "com.h"

int __attribute__((OS_main)) main (void){
  USART_init();
  initReg();
  while (1){
    uint32_t distance = meassureDistance();
    putDec(distance);
    putString("\n\r");
    _delay_ms(100);
  }
  
}
