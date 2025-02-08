/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-22 13:53:47
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-08 17:47:53
 * @FilePath: \Calendar\Drivers\APP\APP_OLED\APP_UI.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __APP_UI_H
#define __APP_UI_H 


// void DisplayShowYearDate(uint8_t x, uint8_t y, Time_s time);
// void DisplayShowTime(uint8_t x, uint8_t y, Time_s time);
// void DisplayShowDay(uint8_t x, uint8_t y, Time_s time);
// uint8_t DisplayShowTempHumi(uint8_t x, uint8_t y, uint32_t tempAndhumi);
error_code_t DisplayShowTempHumi1(void);
void DisplayShowTimeData(void);
error_code_t DisplayShowTempHumi(uint32_t tempAndhumi);
#endif


