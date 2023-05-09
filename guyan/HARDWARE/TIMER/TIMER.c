

#include "TIMER.h"
#include "USART.h"
#include "LED.h"

u8 flag=0;

void TIM3_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʱ��ʹ��

  TIM_TimeBaseStructure.TIM_Period = arr;           	// �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
  TIM_TimeBaseStructure.TIM_Prescaler = psc;					//ʱ��Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);			        // �������жϱ�־ //ԭ�ӵ�STM32F103û��仰
  
	TIM_ITConfig(
								TIM3,
								TIM_IT_Update,
								ENABLE
							);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			//TIM3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_Cmd(TIM3, ENABLE);	//ʹ��TIMx����
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
