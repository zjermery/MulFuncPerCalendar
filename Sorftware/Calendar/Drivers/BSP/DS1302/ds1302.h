#ifndef __DS1302__H
#define __DS1302__H

#include "stdint.h"
#include "stdio.h"
#include  "error_code.h"

/* Size definitions */
#define DS1302_DATA_SIZE            8
#define DS1302_CMD_SIZE             8
#define DS1302_RAM_ADDR_START       0xC0
#define FLAG_VAL                    0xFF

/* Register definition according to the spec */
#define DS1302_REG_SEC              0x80
#define DS1302_REG_MIN              0x82
#define DS1302_REG_HOUR             0x84
#define DS1302_REG_DATE             0x86
#define DS1302_REG_MONTH            0x88
#define DS1302_REG_DAY              0x8A
#define DS1302_REG_YEAR             0x8C
#define DS1302_REG_CONTROL          0x8E
#define D1302_RAM_BURST_MODE        0xFE
#define D1302_CAL_BURST_MODE        0xBE /* Burst mode for calendar */

/************* Configuration section **************/

/* Macros for handling BCD format data stored in the RTC device */
#define BCD_TO_DEC(val)             ((val / 16 * 10) + (val % 16))
#define DEC_TO_BCD(val)             ((val / 10 * 16) + (val % 10))

/* Masks to get fields in the calendar*/
#define MASK_CLOCK_SYSTEM           0x80
#define MASK_CLOCK_PERIOD           0x20
#define MASK_HOURS_24               0x3F
#define MASK_HOURS_12               0x1F
#define MASK_SECONDS                0x7F

/********************************** OLED端口定义 ********************************************************/
/* 引脚 定义 */
#define DS1302_GPIO_PORT                 GPIOC   
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

#define DS1302_CLK_Clr() HAL_GPIO_WritePin(DS1302_CLK_GPIO_PORT,DS1302_CLK_GPIO_PIN,GPIO_PIN_RESET)
#define DS1302_CLK_Set() HAL_GPIO_WritePin(DS1302_CLK_GPIO_PORT,DS1302_CLK_GPIO_PIN,GPIO_PIN_SET)

#define DS1302_DAT_Clr() HAL_GPIO_WritePin(DS1302_DAT_GPIO_PORT,DS1302_DAT_GPIO_PIN,GPIO_PIN_RESET)
#define DS1302_DAT_Set() HAL_GPIO_WritePin(DS1302_DAT_GPIO_PORT,DS1302_DAT_GPIO_PIN,GPIO_PIN_SET)

#define DS1302_RST_Clr() HAL_GPIO_WritePin(DS1302_RST_GPIO_PORT,DS1302_RST_GPIO_PIN,GPIO_PIN_RESET)
#define DS1302_RST_Set() HAL_GPIO_WritePin(DS1302_RST_GPIO_PORT,DS1302_RST_GPIO_PIN,GPIO_PIN_SET)

/************* Configuration section **************/

#define DS1302_RAM_SIZE             31

/**
 * Enumeration for days of the week
 */
typedef enum
{
    MONDAY =  1, /* Valid values 1 - 7*/
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
}Day;

/**
 * Enumeration for clock system (12/24)
 */
typedef enum 
{
    DS1302_CLK_SYSTEM_24,
    DS1302_CLK_SYSTEM_12,
}ClockSystem; 

/**
 * Enumeration for clock period (AM or PM)
 */
typedef enum 
{
    DS1302_CLK_AM_PERIOD,
    DS1302_CLK_PM_PERIOD,
}ClockPeriod;

/**
 * Structure for holding time data
 */
typedef struct 
{
    Day day; /* Range: enum day values*/
    uint8_t sec; /* Range: 0-59 */
    uint8_t min; /* Range: 0-59 */
    uint8_t year; /* Range: 0 - 99 */
    uint8_t hour; /* Range: 1-12/0-23*/
    uint8_t date;  /* Range: 1 - 31 */
    uint8_t month; /* Range: 1 - 12 */
    ClockSystem clockSystem; /* 12 or 24 clock system */
    ClockPeriod clockPeriod; /* AM or PM*/
}Time_s;


void ds1302_init(void);
Time_s *get_time_contx(void);

// void ds1302_get_time(Time_s *time);
Time_s *ds1302_get_time(void);
error_code_t ds1302_set_time(Time_s *time);
void ds1302_clear_ram(void);
uint8_t ds1302_read_ram(const uint8_t addr);
void ds1302_write_ram(const uint8_t addr, uint8_t data);

ClockSystem ds1302_get_clock_system(void);
ClockPeriod ds1302_get_clock_period(void);

void ds1302_write_ram_burst(uint8_t len, uint8_t *buff);
void ds1302_read_ram_burst(uint8_t len, uint8_t *buff);

#endif
