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


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i)   {p->ODR ^=i;} 	 //输出反转状态

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


//传感器数据结构体
typedef struct
{
    float LightVal;//光照强度
}SENSOR;



//系统标志位结构体
typedef struct
{
    u8 BT_Pub;//蓝牙上传消息标志
    uint8_t WarnCount;//报警计数
    uint8_t ClearFlag;
    uint8_t Switch1;//开关1
    uint8_t Switch2;//开关2
    uint8_t Switch3;//开关3
    uint8_t Switch4;//开关4   
}SYSTEM;

typedef struct
{
    int8_t Hour;//时
    int8_t Minute;//分
    int8_t Second;//秒
}ALARM;



extern SENSOR SensorData;//定义传感器结构体
extern SYSTEM System;//定义系统标志位结构体
extern ALARM Alarm1;
extern ALARM Alarm2;
extern ALARM Alarm3;
void NVIC_Config(void);

#endif
