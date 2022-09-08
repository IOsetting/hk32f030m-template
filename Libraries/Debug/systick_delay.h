#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "hk32f030m.h"

void SysTick_Init(void);
void SysTick_DelayUs(uint32_t nus);
void SysTick_DelayMs(uint16_t nms);

#endif
