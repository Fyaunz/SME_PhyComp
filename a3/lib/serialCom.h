#include <avr/io.h>
#include <util/delay.h>

void USART_init(void);
void putString(uint8_t *s);
void put_c(uint8_t data);
void putDec(int32_t x);
uint8_t getChar(void);
void putBin_char(uint8_t c);
void putHex(int32_t data);
