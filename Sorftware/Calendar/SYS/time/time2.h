#ifndef __TIME2_H
#define __TIME2_H
#include "sys.h"

void TIM2_Init(u32 arr,u32 psc);
void TIM2_PWM_Init(u16 arr,u16 psc,u8 ch,uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pinx);
#endif
