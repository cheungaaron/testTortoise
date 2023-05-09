

#include "IWDG.h"


void IWDG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(781);
  IWDG_ReloadCounter();
  IWDG_Enable();
}


