#ifndef __UART_Init_H__
#define __UART_Init_H__

#include <stdint.h>

void UART1_Init(void);
void SystemClock_Config(void);

void UART1_SendByte(uint8_t data);
void UART1_SendString(char *str);

uint8_t UART1_ReceiveByte(void);
void UART1_ReceiveString(char *buffer, uint16_t maxLength);

#endif
