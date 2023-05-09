

#include "TIMER.h"
#include "USART.h"
#include "LED.h"

u8 flag=0;

void TIM3_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//时钟使能

  TIM_TimeBaseStructure.TIM_Period = arr;           	// 自动重装载寄存器周期的值(计数值) 
  TIM_TimeBaseStructure.TIM_Prescaler = psc;					//时钟预分频数 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//向上计数模式
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);			        // 清除溢出中断标志 //原子的STM32F103没这句话
  
	TIM_ITConfig(
								TIM3,
								TIM_IT_Update,
								ENABLE
							);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			//TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_Cmd(TIM3, ENABLE);	//使能TIMx外设
}


void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		flag++;
		flag=flag%2;
		//printf("flag=%d\r\n",flag);
//		if(flag==1)
//			LED_ON;
//		else
//			LED_OFF;
			
  }
 
}
