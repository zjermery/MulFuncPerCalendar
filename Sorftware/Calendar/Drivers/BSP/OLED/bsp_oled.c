/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-11 09:44:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-12 18:51:01
 * @FilePath: \Calendar\Drivers\BSP\OLED\bsp_oled.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
#include "ds1302.h"
#include "stdio.h"
#include "dht11.h"



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



uint8_t DisplayShowTempHumi(uint8_t x, uint8_t y, uint32_t tempAndhumi)
{
    char dateStr[20];  // 假设最大长度不超过20字符
    uint8_t buf[4];
    extern const unsigned char Temperature_and_humidity30x44[];
    if(x > 40 || y > 20 )
    {
        return 1;
    }
      // 将温度值拆分为四个字节
    buf[0] = (tempAndhumi >> 24) & 0xFF;  // 获取最高字节
    buf[1] = (tempAndhumi >> 16) & 0xFF;  // 获取次高字节
    buf[2] = (tempAndhumi >> 8) & 0xFF;   // 获取次低字节
    buf[3] = tempAndhumi & 0xFF;          // 获取最低字节
    OLED_ShowPicture(0,10,30,44,Temperature_and_humidity30x44,1);
    snprintf(dateStr, sizeof(dateStr), "湿度:%02d.%02d%%", buf[0],buf[1]);
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
    y += 25;
    snprintf(dateStr, sizeof(dateStr), "温度:%02d.%02d℃", buf[2],buf[3]);
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
    return 0;
}




