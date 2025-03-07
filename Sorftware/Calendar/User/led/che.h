#ifndef __CHE_H__
#define __CHE_H__

#include "sys.h"

#define QIAN1   GPIO_Pin_5
#define QIAN2   GPIO_Pin_6
#define HOU1    GPIO_Pin_4
#define HOU2    GPIO_Pin_7
#define TUODI   GPIO_Pin_5
#define TUODO   GPIO_Pin_6


#define XIAO_CHE  GPIOA
#define XC_GPIO_CLK  RCC_APB2Periph_GPIOA
#define XIAO_CHE_B  GPIOB
#define XC_GPIO_CLK_B  RCC_APB2Periph_GPIOB


#define QIAN1_OFF       GPIO_ResetBits(XIAO_CHE, QIAN1) //��        
#define QIAN1_ON      GPIO_SetBits(XIAO_CHE, QIAN1)      //��     
#define QIAN2_OFF       GPIO_ResetBits(XIAO_CHE, QIAN2) //��        
#define QIAN2_ON      GPIO_SetBits(XIAO_CHE, QIAN2)      //��   
#define HOU1_OFF       GPIO_ResetBits(XIAO_CHE, HOU1) //��        
#define HOU1_ON      GPIO_SetBits(XIAO_CHE, HOU1)      //��     
#define HOU2_OFF       GPIO_ResetBits(XIAO_CHE, HOU2) //��        
#define HOU2_ON      GPIO_SetBits(XIAO_CHE, HOU2)      //��  
#define TUODI_OFF       GPIO_ResetBits(XIAO_CHE_B, TUODI) //��        
#define TUODI_ON      GPIO_SetBits(XIAO_CHE_B, TUODI)      //��
#define TUODO_OFF       GPIO_ResetBits(XIAO_CHE_B, TUODO) //��        
#define TUODO_ON      GPIO_SetBits(XIAO_CHE_B, TUODO)      //��


void XC_GPIO_Config(void);
void XC_GPIO_Config_B(void);
#endif