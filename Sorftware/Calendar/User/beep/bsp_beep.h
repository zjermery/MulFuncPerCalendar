#ifndef __BEEP_H
#define	__BEEP_H


#include "sys.h" 


/* ��ض˿����ú� */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PORT    	GPIOA						/* GPIO�˿� */
#define BEEP_GPIO_PIN		GPIO_Pin_8					/* ���ӵ�SCLʱ���ߵ�GPIO */







/* �������IO�ĺ� */
#define BEEP_TOGGLE		digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON		    digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF		digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)


void Beep_Init(void);
void Beep_StateRefresh(uint8_t BeepState); 





#endif 
