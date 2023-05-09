#include "Motor.h"
#include "spi.h"
#include "delay.h"


u8 HomingFlag=0;
u8 MidPos_Flag=0;
u8 PLLD_Flag=0;
u64 Start_Point;

void TMC5130A_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //Motor_CS
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	SPI2_Init();		   //初始化SPI2
	SPI_SetSpeed(SPI2,SPI_BaudRatePrescaler_32);	//设置为18M时钟,高速模式
}


void TMC5130A_Current_Setting(void)
{	
	
	/*********** CURRENT  SETTING **************/
	SOFT_SPI_RW(0xa800000004);//GCONF:stealthChop enable
	//SOFT_SPI_RW(0x9000061f14);//IRUN_IHOLD,IRUN:31,IHOLD:20,IHOLDDELAY:6
    SOFT_SPI_RW(0x9000050904);//电流
    
	SOFT_SPI_RW(0x91000000ff);//TPOWERDOWN:255  255
    SOFT_SPI_RW(0xec08010005);//斩波
   // SOFT_SPI_RW(0xF000000037);//电流
	/*********** stealthChop *******************/
	SOFT_SPI_RW(0xf0000101ff);//PWMCONF; pwm_autoscale:0 GRAD:1 AMPL:255 ,35khz
	SOFT_SPI_RW(0xec00010005);//ChopConf:TOFF=5, TBL=2,HSTART=0, HEND=0
	SOFT_SPI_RW(0x93000001f4);//TPWNTHRS;
	SOFT_SPI_RW(0x95000000aa);//THIGH:0xb0(TSTEP:A7)
	SOFT_SPI_RW(0xed00050001);//sgt:5
	//SOFT_SPI_RW(0x94000000b3);//TCOOLTHRS
    //SOFT_SPI_RW(0xA000000000);//位置模式
    
    //SOFT_SPI_RW(0xe900009f00);//e9
    
	/****************** DC STEP****************/
	//SOFT_SPI_RW(0xb300003a98);//VDCMIN:15000
	//SOFT_SPI_RW(0xee00000025);//DCTIME:37
	SOFT_SPI_RW(0xb400000400);//sg_stop
/*********TUNING STEALTH CHOP AND SPREAD CYCLE *************/
	
	
}

void TMC5130A_Speed_Setting(void)
{	
	
        SOFT_SPI_RW(0xa3000005e8); //开始速度
		SOFT_SPI_RW(0xa4000003e8); //A1
    
		SOFT_SPI_RW(0xa500013b80); //V1=5000
		SOFT_SPI_RW(0xa6000011f4); //AMAX=500
       
		SOFT_SPI_RW(0xa7000986a0); //VMAX1=200000

		SOFT_SPI_RW(0xa8000011f4); //DMAX=700
		SOFT_SPI_RW(0xaa000003e8); //D1=1400
		SOFT_SPI_RW(0xab000000fa); //VSTOP=10
		SOFT_SPI_RW(0xa000000000); //RAMPMODE=Positioning mode
    
		
		
}

void TMC5130A_Position(u8 dir,u64 Target)
{	
	u64 Send_Target;
	Send_Target=0;
	//Send_Target=(Send_Target & dir)<<32;
	if(dir == Motor_Back)
		Target = ~Target+1;
		Target = Target & 0x00000000ffffffff;
	Send_Target= 0xad00000000 | Target;
	
	SOFT_SPI_RW(Send_Target);
}


u64 TMC5130A_SPI40_RW(u64 TxData)
{	
	u8 ns;
	u64 RxData;
	u8 TxData_Temp[5];
	u64 RxData_Temp[5];	
	u8 i;
	for(i=0;i<5;i++)
	{
		TxData_Temp[i]=(TxData >> (8*i)) & 0x00000000ff;
	}
	Motor_CS=0;
	ns=ns;
	ns=ns;
	for(i=5;i>0;i--)
	{
		RxData_Temp[i-1]=SPI_ReadWriteByte(SPI2,TxData_Temp[i-1]);
	}
	ns=ns;
	ns=ns;	
	Motor_CS=1;
		//RxData = RxData_Temp[4]<< 32 | RxData_Temp[3]<< 24 | RxData_Temp[2]<< 16 | RxData_Temp[1]<< 8 | RxData_Temp[0] << 0;
	for(i=0;i<5;i++)
	{
		RxData |= RxData_Temp[i]<<(8*i);
	}
	return RxData;
}

void TMC5130A_SOFT_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5 |GPIO_Pin_7;  //Motor_CS Motor_SCK Motor_MOSI
 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  //Motor_MISO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

u64 SOFT_SPI_RW(u64 Txdata)
{
	u8 i;
	u64 Rxdata;
	Rxdata=0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	for(i=0;i<40;i++)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		
	Rxdata <<= 1;
	if(Txdata & 0x8000000000)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
		}
	else 
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		}
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
	{
		Rxdata++;	
	}
	Txdata <<= 1;
	delay_ms(1);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	return Rxdata; 
}





void Motor_Home(void)
{		
		static u8 first=0;
		u32 Zero_Position=0x00000000;
		if (first == 0)
		{
		u8 Stall_Flag;
		SOFT_SPI_RW(0xa000000001); //RAMPMODE=Holding Mode;
		SOFT_SPI_RW(0x6f00000000);	
		while(Stall_Flag != 0x0d )
		{	
			Stall_Flag=(SOFT_SPI_RW(0x6f00000000) & 0xff00000000) >> 32;	//Read out StallFlag	
		}
			HomingFlag=1; //Homing Complete;	
			SOFT_SPI_RW(0xa100000000);	//Set Zero Point;Modify X_ACTUAL;
			first=1;
		}	
		else
		{
			TMC5130A_Position(Motor_Back,Zero_Position);	
		}	
}

void Motor_Move_Mid(void)
{	
	u32 Mid_Position=0x00040000;
	u32 Real_Position;
	SOFT_SPI_RW(0xb400000000); //SW_MODE	
	if(HomingFlag == 1)
	{	
		SOFT_SPI_RW(0xa000000000); //RAMPMODE=Psoitioning mode;
		TMC5130A_Position(Motor_Back,Mid_Position);	
		SOFT_SPI_RW(0x3500000000);//Clear sg_Stop
		SOFT_SPI_RW(0x2100000000);
		while(Mid_Position != Real_Position )
		{
			Real_Position=(u32)(((~SOFT_SPI_RW(0x2100000000))+1) & 0x007fffffff);
		}
		MidPos_Flag=1;
	}
}

//void PLLD(void)
//{		
//		float Press_V=0;
//		
//		/********** Motor Parameters Settings:Slow Mode ************/
////		SOFT_SPI_RW(0xa400000000); //A1=8000
////		SOFT_SPI_RW(0xa500000000); //V1=150000
////		SOFT_SPI_RW(0xa600003e8); //AMAX=5000
////		SOFT_SPI_RW(0xa7000186a0); //VMAX1=50000
////		SOFT_SPI_RW(0xa800003e8); //DMAX=5000
////		SOFT_SPI_RW(0xaa00000000); //D1=8000
////		SOFT_SPI_RW(0xab0000000a); //VSTOP=10
//		//SOFT_SPI_RW(0xa000000000); //RAMPMODE=Positioning mode
//		TMC5130A_Position(Motor_Back,0x00001000);	
//		while(1)
//		{	
//			Press_V=3.3*Press_f/4096;
//			//F_CAN_1_SendData(0x001,Press_f,3);
//			//F_Delay(500);		
//			if(Press_V>1.0060)
//			{	
//				SOFT_SPI_RW(0xa700000000); //VMAX1=0;
//				SOFT_SPI_RW(0xa300000000); //VSTART=0;
//				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//				PLLD_Flag=1;
//				PLLD_Function=0;
//				break;
//			}
//							
//		}	
//}

void Sucking(void)
{		
		u64 Real_X=0;
		u64 Target_X=0;
		TMC5130A_Speed_Setting();	
		SOFT_SPI_RW(0x2100000000);
		Real_X=(~(SOFT_SPI_RW(0x2100000000) & 0x00ffffffff))+1;
		Start_Point=(~(SOFT_SPI_RW(0x2100000000) & 0x00ffffffff))+1;
		Target_X=Real_X+2000;
		TMC5130A_Position(Motor_Back,Target_X);
}

void Spitting(void)
{
		//u64 Real_X=0;
		SOFT_SPI_RW(0x2100000000);
		//Real_X=(~(SOFT_SPI_RW(0x2100000000) & 0x00ffffffff))+1;
		TMC5130A_Position(Motor_Back,Start_Point);
}

//void Sample_Error(void)
//{
//	u8 Blocking_Error=0;
//	u8 Leaking_Error=0;
//	u8 Bubble_Error=0;
//	
//}


void F_Delay(uint32_t delaytime)//ms级									
{
	uint16_t f;
	for(;delaytime>0;delaytime--)				
        		for(f = 10000;f>0;f--);//1ms	
}

