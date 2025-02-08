/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-02-08 09:18:48
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-08 09:44:22
 * @FilePath: \Calendar\Drivers\BSP\OLED\OLED\oled.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
//QDtech-OLED液晶驱动 for STM32
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/8/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
// OLED模块               STM32单片机
//   VCC         接       DC 5V/3.3V      //OLED屏电源正
//   GND         接          GND          //OLED屏电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为4线制SPI
// OLED模块               STM32单片机
//   D1          接          PB15        //OLED屏SPI写信号
//=======================================液晶屏控制线接线==========================================//
// OLED模块               STM32单片机
//   CS          接          PB11        //OLED屏片选控制信号
//   RES         接          PB12        //OLED屏复位控制信号
//   DC          接          PB10        //OLED屏数据/命令选择控制信号
//   D0          接          PB13        //OLED屏SPI时钟信号
//=========================================触摸屏接线=========================================//
//本模块不带触摸功能，所以不需要触摸屏接线
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/	
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
 
//--------------OLED参数定义---------------------
#define PAGE_SIZE    8
#define XLevelL		0x00
#define XLevelH		0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	

//-------------写命令和数据定义-------------------
#define OLED_CMD     0	//写命令
#define OLED_DATA    1	//写数据 
   						  

/********************************** OLED端口定义 ********************************************************/
/* 引脚 定义 */
//SCL
#define OLED_SCL_GPIO_PORT              GPIOA   
#define OLED_SCL_GPIO_PIN               GPIO_PIN_5
#define OLED_SCL_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//SDA
#define OLED_SDA_GPIO_PORT              GPIOA   
#define OLED_SDA_GPIO_PIN               GPIO_PIN_7
#define OLED_SDA_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//RES
#define OLED_RES_GPIO_PORT              GPIOC   
#define OLED_RES_GPIO_PIN               GPIO_PIN_4            
#define OLED_RES_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//DC
#define OLED_DC_GPIO_PORT               GPIOA   
#define OLED_DC_GPIO_PIN                GPIO_PIN_6
#define OLED_DC_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//CS    
#define OLED_CS_GPIO_PORT               GPIOA   
#define OLED_CS_GPIO_PIN                GPIO_PIN_4
#define OLED_CS_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */


/********************************** OLED端口定义END ********************************************************/

#define OLED_SCL_Clr() HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT,OLED_SCL_GPIO_PIN,GPIO_PIN_RESET)//SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT,OLED_SCL_GPIO_PIN,GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT,OLED_SDA_GPIO_PIN,GPIO_PIN_RESET)//SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT,OLED_SDA_GPIO_PIN,GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_PORT,OLED_RES_GPIO_PIN,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_PORT,OLED_RES_GPIO_PIN,GPIO_PIN_SET)

#define OLED_DC_Clr()  HAL_GPIO_WritePin(OLED_DC_GPIO_PORT,OLED_DC_GPIO_PIN,GPIO_PIN_RESET)//DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(OLED_DC_GPIO_PORT,OLED_DC_GPIO_PIN,GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_PORT,OLED_CS_GPIO_PIN,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_PORT,OLED_CS_GPIO_PIN,GPIO_PIN_SET) 

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);     							   		    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Reset(void);
void OLED_Init_GPIO(void);	   							   		    
void OLED_Init(void);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);  
#endif
