

#ifndef __LED_H
#define __LED_H

#include "sys.h"
/*
#define LED_OFF                       GPIOA->BSRR = 0x02
#define LED_ON                        GPIOA->BRR = 0x02
#define LED_TURN                      GPIOA->ODR ^= 0x02
*/


#define LED                 			PAout(4)                			

void LED_Init(void);

#endif
