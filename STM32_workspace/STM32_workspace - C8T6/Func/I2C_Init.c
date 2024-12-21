#include <stdlib.h>
#include <stdint.h>

#include "stm32f10x.h" 
#include <string.h>
#include <stdio.h>


void I2C1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    // 使能 I2C1 和 GPIOB 的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 PB6 (SCL) 和 PB7 (SDA) 为 I2C 的功能引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // I2C1 配置
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;  // 工作模式：I2C
    I2C_InitStructure.I2C_ClockSpeed = 100000;   // I2C 时钟频率 100kHz
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  // 占空比 50%
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;    // 主机不需要地址
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // 7 位地址模式
    I2C_Init(I2C2, &I2C_InitStructure);

    // 启动 I2C1
    I2C_Cmd(I2C2, ENABLE);
}

//************************************I2C1_SendData***********************************************************
void I2C1_SendData(uint8_t addr, uint8_t* data, uint16_t length)
{
	int i1 = 0;
    // 等待 I2C1 就绪
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));  // 等待 I2C 不忙

    // 生成起始条件
    I2C_GenerateSTART(I2C2, ENABLE);

    // 等待起始条件产生完成
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  // 等待开始信号

    // 发送目标设备地址
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter);

    // 等待地址发送完成
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // 发送数组中的数据
    for (i1 = 0; i1 < length; i1++)
    {
        I2C_SendData(I2C2, data[i1]);  // 发送数据

        // 等待数据发送完成
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    // 生成停止条件
    I2C_GenerateSTOP(I2C2, ENABLE);
}

//*************************************I2C1_SendData*********************************************************

//************************************I2C1_ReceiveData**********************************************************
#include "stm32f10x.h"

void I2C1_ReceiveDataArray(uint8_t addr, uint8_t* buffer, uint16_t length)
{
	int i2 = 0;
    // 等待 I2C1 就绪
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));  // 等待 I2C 不忙

    // 生成起始条件
    I2C_GenerateSTART(I2C2, ENABLE);

    // 等待起始条件产生完成
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  // 等待开始信号

    // 发送目标设备地址（接收模式）
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver);

    // 等待地址发送完成
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // 接收数据
    for (i2 = 0; i2 < length; i2++)
    {
        // 等待数据接收完成
        while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));

        // 读取接收到的数据并存入 buffer
        buffer[i2] = I2C_ReceiveData(I2C2);
        
        // 如果是接收到倒数第二个字节，准备停止条件
        if (i2 == length - 2) 
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE);  // 禁用 ACK
        }
    }

    // 生成停止条件
    I2C_GenerateSTOP(I2C2, ENABLE);
    
    // 重新启用 ACK，以便接收下次数据
    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

//************************************I2C1_ReceiveData**********************************************************
