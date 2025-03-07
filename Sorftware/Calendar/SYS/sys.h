#ifndef __SYS_H
#define __SYS_H	

#include "stm32f10x.h"
#include "bsp_config.h"
#include "./delay/delay.h"																    
#include "./my_usart/my_usart1.h"	
#include "./my_usart/my_usart2.h"
#include "./my_usart/usart3.h"
#include "stdio.h"      
#include "stdarg.h"		
#include "string.h"
#include "./wifi/wifi.h"
//#include "./uarst3/usart3.h"	
//#include "./uarst3/gps.h"	
#include "./time/time4.h"
#include "./time/time3.h"
#include "./time/time2.h"
#include "./time/time1.h"
//#include "./hc_sr04/hc_sr04.h"
#include "./led/led.h"
#include "main.h"
//#include "./led/gy906.h"
#include "./key/key.h"
//#include "./mems/mems.h"
//#include "./mems/memss.h"
#include "./mqtt/mqtt.h"
#include "./mqtt/utils_hmac.h"
#include "./mqtt/utils_md5.h"
#include "./mqtt/utils_sha1.h"

#include "math.h"

#include "./oled/bsp_oled_iic.h"
#include "./oled/oledFont.h"
#include "./beep/bsp_beep.h"
#include "./RTC/rtc.h"
#include "./dht11/DHT11.h"

#include "./gy30/gy30.h"
#include "./gy30/gy30iic.h"
#include "MyRTC.h"  
#include "./menu/menu.h"


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i)   {p->ODR ^=i;} 	 //�����ת״̬

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����


//���������ݽṹ��
typedef struct
{
    float LightVal;//����ǿ��
}SENSOR;



//ϵͳ��־λ�ṹ��
typedef struct
{
    u8 BT_Pub;//�����ϴ���Ϣ��־
    uint8_t WarnCount;//��������
    uint8_t ClearFlag;
    uint8_t Switch1;//����1
    uint8_t Switch2;//����2
    uint8_t Switch3;//����3
    uint8_t Switch4;//����4   
}SYSTEM;

typedef struct
{
    int8_t Hour;//ʱ
    int8_t Minute;//��
    int8_t Second;//��
}ALARM;



extern SENSOR SensorData;//���崫�����ṹ��
extern SYSTEM System;//����ϵͳ��־λ�ṹ��
extern ALARM Alarm1;
extern ALARM Alarm2;
extern ALARM Alarm3;
void NVIC_Config(void);

#endif
