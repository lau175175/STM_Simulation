#ifndef __I2C_Init_H__
#define __I2C_Init_H__

#include <stdint.h>

void I2C1_Init(void);

void I2C1_SendData(uint8_t addr, uint8_t* data, uint16_t length);
void I2C1_ReceiveDataArray(uint8_t addr, uint8_t* buffer, uint16_t length);

#endif

