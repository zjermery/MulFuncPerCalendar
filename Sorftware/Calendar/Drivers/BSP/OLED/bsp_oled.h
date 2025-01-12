/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-11 09:44:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-12 18:49:55
 * @FilePath: \Calendar\Drivers\BSP\OLED\bsp_oled.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __BPS_OLED_H
#define __BSP_OLED_H 


void DisplayShowYearDate(uint8_t x, uint8_t y, Time_s time);
void DisplayShowTime(uint8_t x, uint8_t y, Time_s time);
void DisplayShowDay(uint8_t x, uint8_t y, Time_s time);
void DisplayShowHum(uint8_t x, uint8_t y, uint8_t temp);
void DisplayShowTemp(uint8_t x, uint8_t y, uint8_t hum);

#endif


