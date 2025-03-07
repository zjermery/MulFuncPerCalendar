#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
  	 

extern u8 isKey1;
extern u8 isKey2;
extern u8 isKey3;
extern u8 isKey4;
extern u8 isKey5;
extern u8 isKey6;
extern u8 isKey7;
extern u8 isKey8;

/* ��ض˿����ú� */
#define KEY1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define KEY1_GPIO_PORT    	GPIOB						/* GPIO�˿� */
#define KEY1_GPIO_PIN		GPIO_Pin_12					/* ���ӵ�SCLʱ���ߵ�GPIO */


#define KEY2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define KEY2_GPIO_PORT    	GPIOB						/* GPIO�˿� */
#define KEY2_GPIO_PIN		GPIO_Pin_13					/* ���ӵ�SCLʱ���ߵ�GPIO */

#define KEY3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define KEY3_GPIO_PORT    	GPIOB						/* GPIO�˿� */
#define KEY3_GPIO_PIN		GPIO_Pin_14					/* ���ӵ�SCLʱ���ߵ�GPIO */

#define KEY4_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define KEY4_GPIO_PORT    	GPIOB						/* GPIO�˿� */
#define KEY4_GPIO_PIN		GPIO_Pin_15					/* ���ӵ�SCLʱ���ߵ�GPIO */


#define KEY5_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define KEY5_GPIO_PORT    	GPIOB						/* GPIO�˿� */
#define KEY5_GPIO_PIN		GPIO_Pin_1					/* ���ӵ�SCLʱ���ߵ�GPIO */







#define KEY1 	GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)//��ȡ����1
#define KEY2  	GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN)//��ȡ����2
#define KEY3  	GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN) //��ȡ����3
#define KEY4  	GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN) //��ȡ����4

#define KEY5 	GPIO_ReadInputDataBit(KEY5_GPIO_PORT,KEY5_GPIO_PIN)//��ȡ����5



void KEY_Init(void);//IO��ʼ��
void Key1Press(void);
void Key2Press(void);
void Key3Press(void);
void Key4Press(void);

void Key5Press(void);
void Key6Press(void);
void Key7Press(void);
void Key8Press(void);

void KeyScan(void);
#endif
