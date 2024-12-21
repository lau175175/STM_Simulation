#include <math.h> 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"                  // Device header
#include "I2C_Init.h"

#define SlaveAddress 0xc0  // 还是需要7bit设备地址带读写位0/1
#define MCP4725_Mode 0x60  //配置正常的IIC模式

void MCP4725_Write_FastMode(float AnalogValue)  //fast mode Fuc
{
	int DigValue1 = (int)round((AnalogValue)/3.3 * 4095);  // 数模转换
	
	uint16_t DigValue2 = (uint16_t)DigValue1;
	uint8_t DigValue3[2];
	DigValue3[0] = (uint8_t)(DigValue2 >> 8);
	DigValue3[1] = (uint8_t)(DigValue2 & 0xFF);
	
	I2C1_SendData(SlaveAddress, DigValue3, sizeof(DigValue3));  // 直接打包发送字节串
}

void MCP4725_Write_NormalMode(float analog_Data)  //normal mode Fuc
{
	int DigValue1 = (int)round((analog_Data / 3.3) * 4095);

	uint16_t DigValue2 = (uint16_t)DigValue1;
	uint8_t DigValue3[3];
	DigValue3[0] = MCP4725_Mode;
	DigValue3[1] = (uint8_t)((DigValue2 >> 4) & 0xFF);
	DigValue3[2] = (uint8_t)((DigValue2 << 4) & 0xFF);
	
	I2C1_SendData(SlaveAddress, DigValue3, sizeof(DigValue3));
}

void MCP4725_Stop(void)  //停止输出电压
{
	uint8_t StopData[] = {0x10, 0x00};   //停止输出的命令
	
	I2C1_SendData(SlaveAddress, StopData, sizeof(StopData));  //sizeof是指针的大小
}
















