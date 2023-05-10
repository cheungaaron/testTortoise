
#include "sys.h"
#include "USART.h"
#include "delay.h"
#include "LED.h"
#include "IWDG.h"
#include "TIMER.h"
#include "EXTI.h"
#include "motor.h"


//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
int main(void)
{
	delay_init();

	//	LED_Init();
	//	LED=1;	
	//	delay_ms(300);	
	//	LED=0;	
	//	delay_ms(300);
	//	LED=1;
	//	delay_ms(300);	
	//	LED=0;	
	delay_ms(300);									//开机闪两下确定LED正确

	USART1_Init(9600);
	printf("STM32F030F4P6已经开机。\r\n"); 	//确定串口发送正确

	//EXTI_PA7_Init();
	//TIM3_Init(4999,4799);
	//TMC5130A_SPI_Init();
	TMC5130A_SOFT_SPI_Init();
	TMC5130A_Current_Setting();
	TMC5130A_Speed_Setting();
	delay_ms(1500);

	//TMC5130A_Position(1,3200);
	//Motor_Move_Mid();
	//	IWDG_Init();	
	//	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	//	{
	//	  printf("IWDG复位\r\n");
	//	  RCC_ClearFlag();
	//	}
	while (1)
		{

		// SOFT_SPI_RW(0xa7000986a0);
		SOFT_SPI_RW(0xAd004e2000);
		delay_s(15);

		//SOFT_SPI_RW(0xa7000086a0);
		SOFT_SPI_RW(0xAd00000000);
		delay_s(15);
		printf("STM32F030F4P6正在运行。\r\n");

		//IWDG_ReloadCounter();    //去掉注解“喂狗” 
		}







}


/*****END OF FILE****/
