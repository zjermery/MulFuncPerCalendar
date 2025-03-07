#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define LED_GPIO_PIN1   GPIO_Pin_13
#define wifi_rst_PIN        GPIO_Pin_9

#define LED_GPIO_PORT  GPIOC
#define LED_GPIO_CLK  RCC_APB2Periph_GPIOC


#define WIFI_GPIO_PORT  GPIOB
#define WIFI_GPIO_CLK  RCC_APB2Periph_GPIOB

#define LED1_OFF       GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN1) //关        
#define LED1_ON      GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN1)      //开     


#define WIFI_RST     GPIO_SetBits(WIFI_GPIO_PORT, wifi_rst_PIN) 

#define REDSW PAin(5)
#define REDSW1 PBin(6)

#define DESIN_OFF       GPIO_ResetBits(GPIOB, GPIO_Pin_9) //关        
#define DESIN_ON      GPIO_SetBits(GPIOB, GPIO_Pin_9)      //开 

void LED_StateRefresh(uint8_t LEDState); 
void RedSwitch_Init(void);
void Disinfect_Init(void);


void LED_Sta(void);

void LED_GPIO_Config(void);
void wifi_GPIO_Config(void);

#endif

