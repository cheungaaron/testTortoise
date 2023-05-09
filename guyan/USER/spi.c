#include "spi.h"
#include "Motor.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	  							  
//SPI口初始化
//SPI1,SPI2初始化
//SPI1,SPI2速度选择
//SPI1,SPI2 读写
//2018,8.24 



void SPI1_Init(void)  //FRAM,数字电位器1，数字电位器2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI_ReadWriteByte(SPI1,0xff);//启动传输		 
} 

void SPI2_Init(void) //TMC5130A电机驱动
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );	
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI_ReadWriteByte(SPI2,0xff);//启动传输		 
} 
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
  
void SPI_SetSpeed(SPI_TypeDef* SPI12,u8 SpeedSet) //SPI1 or SPI2,Speed
{	
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI12, &SPI_InitStructure);
	SPI_Cmd(SPI12,ENABLE);
}


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI_ReadWriteByte(SPI_TypeDef* SPI12,u8 TxData) //SPI1 or SPI2,Data
{	
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI12, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_SendData8(SPI12, TxData); //通过外设SPIx发送一个数据
	retry=0;
//	delay_ms(1);
	while (SPI_I2S_GetFlagStatus(SPI12, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	//	while(SPI_I2S_GetFlagStatus(SPI12, SPI_I2S_FLAG_BSY) == SET)
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_ReceiveData8(SPI12); //返回通过SPIx最近接收的数据					    
}



