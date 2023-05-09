#include "sys.h"

#define Motor_Forward 								  0x00  //电机正转
#define Motor_Back										  0x01  //电机反转
#define Motor_CS PBout(12)
#define Motor_SCK PBout(13)
void TMC5130A_SPI_Init(void);
void TMC5130A_Current_Setting(void);
u64	TMC5130A_SPI40_RW(u64 TxData);
void TMC5130A_Speed_Setting(void);
void TMC5130A_SOFT_SPI_Init(void);
void TMC5130A_Position(u8 dir,u64 Target);
void SOFT_SPI_Write(u64 Txdata);
u64 SOFT_SPI_Read(void);
u64 SOFT_SPI_RW(u64 Txdata);
void F_Delay(uint32_t delaytime);//ms级	
void Motor_Home(void);
void Motor_Move_Mid(void);
void PLLD(void);
void Sucking(void);
void Spitting(void);
extern u8 HomingFlag;

