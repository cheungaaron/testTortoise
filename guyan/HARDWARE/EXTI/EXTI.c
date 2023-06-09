
#include "EXTI.h"
#include "USART.h"   // USASRT1驱动模块头
#include "LED.h"



void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void EXTI_PA7_Init(void)
{ 
     EXTI_InitTypeDef EXTI_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
             
     KEY_Init();         
     
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);

     EXTI_ClearITPendingBit(EXTI_Line7);
   
     EXTI_InitStruct.EXTI_Line = EXTI_Line7;
     EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
     EXTI_InitStruct.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStruct); 

     NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
     NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStruct);
}



//=============================================================================
//文件名称：EXTI4_15_IRQHandler
//功能概要：按键中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void EXTI4_15_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line7)!= RESET ) 
	{		
		printf("PA7响应一次下降沿触发中断\r\n");
		EXTI_ClearITPendingBit(EXTI_Line7);
	}		
	
	
}	
	
