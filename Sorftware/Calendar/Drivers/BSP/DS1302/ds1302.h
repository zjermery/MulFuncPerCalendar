#ifndef __SPI_H
#define __SPI_H
#include "sys.h"



/********************************** OLED端口定义 ********************************************************/
/* 引脚 定义 */
//CLK
#define DS1302_CLK_GPIO_PORT              GPIOC   
#define DS1302_CLK_GPIO_PIN               GPIO_PIN_9
#define DS1302_CLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */
//DAT
#define DS1302_DAT_GPIO_PORT              GPIOC   
#define DS1302_DAT_GPIO_PIN               GPIO_PIN_8
#define DS1302_DAT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */
//RST
#define DS1302_RST_GPIO_PORT              GPIOC   
#define DS1302_RST_GPIO_PIN               GPIO_PIN_7
#define DS1302_RST_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

#define DS1302_CLK_Clr() HAL_GPIO_WritePin(DS1302_CLK_GPIO_PORT,DS1302_CLK_GPIO_PIN,GPIO_PIN_RESET)//SCL
#define DS1302_CLK_Set() HAL_GPIO_WritePin(DS1302_CLK_GPIO_PORT,DS1302_CLK_GPIO_PIN,GPIO_PIN_SET)

#define DS1302_DAT_Clr() HAL_GPIO_WritePin(DS1302_DAT_GPIO_PORT,DS1302_DAT_GPIO_PIN,GPIO_PIN_RESET)//SCL
#define DS1302_DAT_Set() HAL_GPIO_WritePin(DS1302_DAT_GPIO_PORT,DS1302_DAT_GPIO_PIN,GPIO_PIN_SET)

#define DS1302_RST_Clr() HAL_GPIO_WritePin(DS1302_RST_GPIO_PORT,DS1302_RST_GPIO_PIN,GPIO_PIN_RESET)//SCL
#define DS1302_RST_Set() HAL_GPIO_WritePin(DS1302_RST_GPIO_PORT,DS1302_RST_GPIO_PIN,GPIO_PIN_SET)




uint8_t read_data_from_1302(uint8_t add);								 
void write_data_to_add(uint8_t add,uint8_t dat);			 
void ds1302_init(void);
void ds1302_print_data(uint8_t *read);
void Sendchar(uint8_t com);
uint8_t Readchar(void);
#endif

