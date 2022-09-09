#include <avr/io.h>
#include <util/delay.h>
#include "serialCom.h"
#include "i2c.h"
#define BLINK_DELAY_MS 200

volatile int16_t disStart = 0;
volatile int16_t disEnd = 0;
volatile uint64_t disbuffer[64];

ISR(TIMER1_CAPT_vect){

    uint64_t duration =  ICR1;
    duration = duration / 2;
    disbuffer[disEnd] = ((duration * 34) / 2000) -5;
    disEnd = (disEnd + 1) & 63;
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

void ping(void){
    PORTD |= (1<<PORTD3);
    _delay_us(10);
    PORTD &= ~(1<<PORTD3);
    ICR1 &= 0x0000;
    TCNT1 &= 0x0000;
}

void initReg(void){
  DDRD = (1<<DDD3);
  ICR1 &= 0x0000;
}

uint32_t meassureDistance(void){

  ping();

  if(disStart != disEnd){
    uint32_t distance = disbuffer[disStart];
    disStart = (disStart + 1) & 63;
    if(distance > 4 && distance < 300){
      return distance;
    }
  } 

  ICR1 &= 0x0000;

}
