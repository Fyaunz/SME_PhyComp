#include <avr/io.h>
#include <util/delay.h>
#include "serialCom.h"
#include "i2c.h"
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

ISR(INT0_vect){
  
}

void init_IMU(void){

  I2C_init;

  I2C_poke(MPU_6050, 0x38, 0x01);
  I2C_poke(MPU_6050, 0x6B, 0x00);
  I2C_poke(MPU_6050, 0x6C, 0x00);
  I2C_poke(MPU_6050, 0x6A, 0x00);
  I2C_poke(MPU_6050, 0x19, 0x14);

}

void read_all(void){

  init_IMU();
  uint8_t data[120];

  for(int i = 0; i<120; i++){
    data[i] = 0;
  }

  I2C_read_registers(MPU_6050, 0x0D, data, 105);

  for(int i = 0; i<120; i++){
    putDec(i + 13);
    put_c(':');
    putBin_char(data[i]);
    putString("\n\r");
  }  
}
