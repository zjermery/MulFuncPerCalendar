#ifndef __BPS_OLED_H
#define __BSP_OLED_H 


void DisplayShowYearDate(uint8_t x, uint8_t y, Time_s time);
void DisplayShowTime(uint8_t x, uint8_t y, Time_s time);
void DisplayShowDay(uint8_t x, uint8_t y, Time_s time);
uint8_t DisplayShowTempHumi(uint8_t x, uint8_t y, uint32_t tempAndhumi);

#endif


