#include "stdint.h"
#include "stm32f10x.h"

//******************************************SystemClock_Config*******************************************************************
void SystemClock_Config(void)
{
    // 1. 配置外部高速晶振（HSE）
    RCC->CR |= RCC_CR_HSEON;  // 打开 HSE
    while (!(RCC->CR & RCC_CR_HSERDY));  // 等待 HSE 就绪

    // 2. 配置 PLL（使用 HSE 作为 PLL 源，倍频系数为 9）
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;  // 选择 HSE 作为 PLL 输入
    RCC->CFGR &= ~RCC_CFGR_PLLMULL;  // 清除 PLL 倍频设置
    RCC->CFGR |= RCC_CFGR_PLLMULL9;  // 设置 PLL 倍频系数为 9

    RCC->CR |= RCC_CR_PLLON;  // 打开 PLL
    while (!(RCC->CR & RCC_CR_PLLRDY));  // 等待 PLL 就绪

    // 3. 配置系统时钟源为 PLL
    RCC->CFGR &= ~RCC_CFGR_SW;  // 清除现有的 SYSCLK 源
    RCC->CFGR |= RCC_CFGR_SW_PLL;  // 选择 PLL 作为系统时钟源
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // 等待 PLL 成为系统时钟

    // 4. 配置 AHB、APB1、APB2 时钟
    RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB 时钟分频系数为 1
    RCC->CFGR |= RCC_CFGR_PPRE1_1;  // APB1 时钟分频系数为 2 (HCLK/2)
    RCC->CFGR |= RCC_CFGR_PPRE2_1;  // APB2 时钟分频系数为 1 (HCLK/1)

    // 5. 配置闪存延迟
    FLASH->ACR |= FLASH_ACR_LATENCY_2;  // 设置闪存延迟为 2

    // 6. 配置外设时钟（例如 ADC 时钟）
    RCC->CFGR &= ~RCC_CFGR_ADCPRE;  // ADC 时钟选择
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;  // 设置 ADC 时钟为 PCLK2/6
}
//*****************************************SystemClock_Config********************************************************************

// ***************************************************UART1配置 PA9/PA10***********************************************************
void UART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  //定义结构体
    USART_InitTypeDef USART_InitStructure;  //定义结构体
    
    // 开启GPIOA和USART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 启用GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 启用USART1时钟
    
    // 配置PA9为USART1的TX引脚（复用推挽输出）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置PA10为USART1的RX引脚（浮空输入）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // USART1配置
    USART_InitStructure.USART_BaudRate = 115200; // 设置波特率115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 数据位长度8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No; // 无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 启用接收和发送模式
    
    // 初始化USART1
    USART_Init(USART1, &USART_InitStructure);
    
    // 启用USART1
    USART_Cmd(USART1, ENABLE);
}
// ***************************************************UART1配置 PA9/PA10***********************************************************

//*******************************UART1_Send*********************************************
// 发送一个字节
void UART1_SendByte(uint8_t data)
{
    // 等待USART发送数据寄存器空
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // 检查发送缓冲区是否空
    
    // 发送数据
    USART_SendData(USART1, data);
}

// 发送字符串
void UART1_SendString(char *str) 
{
	if(str && *str)  //检查指针和字符串都不为空
	{
		while (*str) {
        UART1_SendByte(*str++);  // 逐字符发送
		}
		UART1_SendByte('\r');
		UART1_SendByte('\n');
	}
}

//******************************UART1_Send*************************************************

//******************************UART1_Receive*******************************************************

// 接收一个字节
uint8_t UART1_ReceiveByte(void) 
{
    // 等待直到接收缓冲区有数据
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);  // 检查接收缓冲区是否非空
    
    // 从接收数据寄存器读取接收到的数据
    return USART_ReceiveData(USART1);
}

// 接收字符串直到换行符
void UART1_ReceiveString(char *buffer, uint16_t maxLength) 
{
    uint16_t i = 0;
    char ch;
    
    while (i < maxLength) {
        ch = UART1_ReceiveByte();  // 接收一个字节
        if (ch == '\n') {
            break;  // 遇到换行符停止接收
        }
        buffer[i++] = ch;  // 将接收到的字符存入缓冲区
    }
    buffer[i] = '\0';  // 添加字符串结束符
}

//********************************UART1_Receive*****************************************************

