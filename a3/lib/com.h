#include <avr/io.h>
#include <util/delay.h>
#include "serialCom.h"
#include "i2c.h"
#define BLINK_DELAY_MS 200

void init_IMU(void);
void read_all(void);
void initReg(void);
uint32_t meassureDistance(void);
