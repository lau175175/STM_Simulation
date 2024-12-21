#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#include "stm32f10x.h"                  // Device header

#include "ADC_Init.h"
#include "UART_Init.h"
#include "I2C_Init.h"
#include "MCP4725.h"
#include "Delay_ms.h"
#include "I2C_Init.h"

int main(void)
{
	UART1_Init();
	SystemClock_Config();
	ADC1_Init();
	I2C1_Init();
	
	char buffer[50];
	float ADCValue = 0.0;
	char ADCValueInfo[50];
	char ADCValueInfo1[50] = "175";
	float VoltageValue = 2.7;
	
	/*
	uint8_t i = 0;  // 迭代计数用
	float Va = 0.0;  // ADC采样PD值
	float Vb = 0.0;  // 未工作PD要加的电压
	char MessageVo[50];  //用于UART
	*/
	
	while(1)
	{		
		UART1_SendString(ADCValueInfo1);
		delay_ms(100);
		ADCValue = Get_ADC_Value();
		delay_ms(100);
		sprintf(ADCValueInfo, "ADC: %.3fV", ADCValue);
		delay_ms(100);
		UART1_SendString(ADCValueInfo);
		delay_ms(100);

		/*
		MCP4725_Write_FastMode(VoltageValue);
		delay_ms(500);
		MCP4725_Stop();
		delay_ms(500);
		UART1_SendString(ADCValueInfo1);
		*/
				
		/* UART
		UART1_ReceiveString(buffer,strlen(buffer));
		delay_ms(100);
		UART1_SendString(buffer);
		delay_ms(100);
		*/	

		/**********************************************************************************
		MCP4725_Stop();  //第一次的Vo采样基于Vo'悬空，不加电压
		delay_ms(100);
		Va = Get_ADC_Value();  //获取Vo的值
		
		if(i == 0)
		{
			Vb = Va;
			MCP4725_Write_FastMode(Vb);
			delay_ms(100);
			Va = Get_ADC_Value();
			
			while((Va - Vb)/Va > 0.03)  // 如果误差不在需求范围内就一直迭代
			{
				Vb = Va;
				MCP4725_Write_FastMode(Vb);
				delay_ms(100);
				Va = Get_ADC_Value();
			}
			
			i = i + 1;
		}
		
		if(i < 2)  //再验证一次是不是 < 3%
		{
			Vb = Va;
			MCP4725_Write_FastMode(Vb);
			delay_ms(100);
			Va = Get_ADC_Value();
		}
		
		else
		{
			MCP4725_Stop();
			delay_ms(100);
			
			sprintf(MessageVo, "Vo: %.3f.V", Va);
			UART1_SendString(MessageVo);
		}
		*/
	}
}



