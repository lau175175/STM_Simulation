#include <stdint.h>

#include "stm32f10x.h"                  // Device header
#include "math.h"
#include "Delay_ms.h"

float ADC_Value = 0.0;
uint16_t adcValue;

void ADC1_Init(void)
{
	// 定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
    // 1. 启用 ADC1 和 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   // 启用 ADC1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   // 启用 GPIOB 时钟

    // 2. 配置 PB0 为模拟输入模式（ADC1 通道 8）

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  // 配置 PB0
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 3. 配置 ADC1
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 设置 ADC 时钟为 APB2 时钟 / 6，即 12 MHz，配置 ADC 时钟的预分频器
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;      // 独立模式
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;           // 非扫描模式，单通道转换
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;     // 连续转换模式【重要】
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐
    ADC_InitStruct.ADC_NbrOfChannel = 1;                 // 单通道
    ADC_Init(ADC2, &ADC_InitStruct);

    // 4. 配置 ADC1 通道 8
    ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_1Cycles5); // 配置通道 8，采样时间 55.5 个时钟周期

	// 5. 启用 ADC1
    ADC_Cmd(ADC2, ENABLE);  // 启用 ADC1

    // 6. 校准 ADC1
    ADC_ResetCalibration(ADC2);
	delay_ms(100);
    // while (ADC_GetResetCalibrationStatus(ADC2));  // 等待校准完成
    ADC_StartCalibration(ADC2);
    // while (ADC_GetCalibrationStatus(ADC2));  // 等待校准完成
	delay_ms(100);
	
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);  // 触发ADC转换
	
}

//*********************************Get_ADC_Value******************************************************************
float Get_ADC_Value(void)
{
	delay_ms(100);
    // 读取 ADC 转换结果
    adcValue = ADC_GetConversionValue(ADC2);  // 读取 ADC 数据寄存器中的结果
	
	ADC_Value = (adcValue * 3.3) / 4095;  // 基于12位的分辨率

    return ADC_Value;  // 返回 ADC 值
}
//*********************************Get_ADC_Value***********************************************************************
