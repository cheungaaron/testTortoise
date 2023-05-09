#ifndef _DELAY_H_
#define _DELAY_H_

#include "sys.h"
void delay_init(void);
void delay_us(int32_t nus);
void delay_ms(int16_t nms);
void delay_s(uint16_t ns);
#endif
