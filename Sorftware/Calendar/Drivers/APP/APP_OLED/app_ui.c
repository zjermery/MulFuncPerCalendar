/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-11 09:44:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-08 18:28:36
 * @FilePath: \Calendar\Drivers\BSP\OLED\bsp_oled.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
#include "gui.h"
#include "ds1302.h"
#include "stdio.h"
#include "dht11.h"
#include "string.h"
#include "error_code.h"


typedef enum{
    OLED_COLOR_INVALID = 0,
    OLED_COLOR_NORMAL
}OLED_COLOR_Mode;


#define FONT_SIZE 16

#define CURSOR_FLASH_TIME 500

char weeks[7][10] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};



void DisplayshowCursor(void)
{
  OLED_Clear(0);

  static uint32_t startTime = 0;
  uint32_t difftime = HAL_GetTick() - startTime;

  if(difftime > 2 * CURSOR_FLASH_TIME)
  {
    startTime = HAL_GetTick();
  }
  else if(difftime > CURSOR_FLASH_TIME)
  {
    GUI_DrawLine(24,17, 56, 17, OLED_COLOR_NORMAL);
  }
  
  OLED_Display();

}


void DisplayShowTimeData(void)
{
    Time_s *time = ds1302_get_time();
    // 计算日期字符串
    char dateStr[30];  // 假设最大长度不超过20字符
    snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d", (time->year + 2000), time->month, time->date);
    // 使用OLED_ShowString一次性显示日期字符串
    GUI_ShowString(24, 0, (uint8_t *)dateStr, 8, OLED_COLOR_NORMAL);

    snprintf(dateStr, sizeof(dateStr), "%02d:%02d:%02d", time->hour, time->min, time->sec);
    // 使用OLED_ShowString一次性显示日期字符串
    GUI_ShowString(16, 20, (uint8_t *)dateStr, 16, OLED_COLOR_NORMAL);

    char *week = weeks[time->day+1];
    uint8_t x_week = (128 - (strlen(week) * 8))/2;
    // 使用OLED_ShowString一次性显示日期字符串
    GUI_ShowString(16, 48, (uint8_t*)week, 16, OLED_COLOR_NORMAL);

}


error_code_t DisplayShowTempHumi1(void)
{
    char dateStr[20];  // 假设最大长度不超过20字符
    uint32_t tempAndhumi;
    uint8_t buf[4];
    extern const unsigned char Temperature_and_humidity30x44[]; 
    DHT11_Read_Data(&tempAndhumi);
      // 将温度值拆分为四个字节
    buf[0] = (tempAndhumi >> 24) & 0xFF;  // 获取最高字节
    buf[1] = (tempAndhumi >> 16) & 0xFF;  // 获取次高字节
    buf[2] = (tempAndhumi >> 8) & 0xFF;   // 获取次低字节
    buf[3] = tempAndhumi & 0xFF;          // 获取最低字节
    GUI_DrawPicture(0,10,30,44,Temperature_and_humidity30x44,1);
    snprintf(dateStr, sizeof(dateStr), "湿度:%02d.%02d%%", buf[0],buf[1]);
    GUI_ShowCHineseAndASCII(32, 10, (uint8_t*)dateStr, OLED_COLOR_NORMAL);
    snprintf(dateStr, sizeof(dateStr), "温度:%02d.%02d℃", buf[2],buf[3]);
    GUI_ShowCHineseAndASCII(32, 35, (uint8_t*)dateStr, OLED_COLOR_NORMAL);
    
    return ERROR_CODE_SUCCESS;
}


error_code_t DisplayShowTempHumi(uint32_t tempAndhumi)
{
    char dateStr[20];  // 假设最大长度不超过20字符
    uint8_t buf[4];
    extern const unsigned char Temperature_and_humidity30x44[]; 

      // 将温度值拆分为四个字节
    buf[0] = (tempAndhumi >> 24) & 0xFF;  // 获取最高字节
    buf[1] = (tempAndhumi >> 16) & 0xFF;  // 获取次高字节
    buf[2] = (tempAndhumi >> 8) & 0xFF;   // 获取次低字节
    buf[3] = tempAndhumi & 0xFF;          // 获取最低字节
    GUI_DrawPicture(0,10,30,44,Temperature_and_humidity30x44,1);
    snprintf(dateStr, sizeof(dateStr), "湿度:%02d.%02d%%", buf[0],buf[1]);
    GUI_ShowCHineseAndASCII(32, 10, (uint8_t*)dateStr, OLED_COLOR_NORMAL);

    snprintf(dateStr, sizeof(dateStr), "温度:%02d.%02d℃", buf[2],buf[3]);
    GUI_ShowCHineseAndASCII(32, 36, (uint8_t*)dateStr, OLED_COLOR_NORMAL);
    
    return ERROR_CODE_SUCCESS;
}




