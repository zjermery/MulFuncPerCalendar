/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-11 09:44:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-11 14:42:37
 * @FilePath: \Calendar\Drivers\BSP\OLED\bsp_oled.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
 #include "ds1302.h"
 #include "stdio.h"

#define SHOW_MODE 1    
#define FONT_SIZE 16



void DisplayShowYearDate(uint8_t x, uint8_t y, Time_s time)
{
    // 计算日期字符串
    char dateStr[20];  // 假设最大长度不超过20字符
    snprintf(dateStr, sizeof(dateStr), "%04d年%02d月%02d日", (time.year + 2000), time.month, time.date);
    // 使用OLED_ShowString一次性显示日期字符串
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}

void DisplayShowTime(uint8_t x, uint8_t y, Time_s time)
{
    // 计算日期字符串
    char dateStr[20];  // 假设最大长度不超过20字符
    snprintf(dateStr, sizeof(dateStr), "%02d时%02d分%02d秒", time.hour, time.min, time.sec);
    // 使用OLED_ShowString一次性显示日期字符串
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}


void DisplayShowDay(uint8_t x, uint8_t y, Time_s time)
{
    // 计算日期字符串
    char dateStr[20];  // 假设最大长度不超过20字符
    switch (time.day)
    {
    case MONDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期一");
        break;
    case TUESDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期二");
        break;
    case WEDNESDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期三");
        break;
    case THURSDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期四");
        break;    
    case FRIDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期五");
        break;
    case SATURDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期六");
        break;
    case SUNDAY:  
        snprintf(dateStr, sizeof(dateStr), "星期日");
        break;
    default:
        snprintf(dateStr, sizeof(dateStr), "星期日");
        break;
    }
    // 使用OLED_ShowString一次性显示日期字符串
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}

