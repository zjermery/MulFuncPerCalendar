
#ifndef __CALENDAR_H
#define __CALENDAR_H

#include "rtc.h"


uint8_t GetMoonDay(uint8_t month_p,unsigned short table_addr);
uint8_t GetChinaCalendar(uint16_t  year,uint8_t month,uint8_t day,uint8_t *p);
void GetSkyEarth(uint16_t year,uint8_t *p);
void StrCopy(uint8_t *target,uint8_t const *source,uint8_t no);
void GetChinaCalendarStr(uint16_t year,uint8_t month,uint8_t day,uint8_t *str);
uint8_t GetJieQi(uint16_t year,uint8_t month,uint8_t day,uint8_t *JQdate);
uint8_t GetJieQiStr(uint16_t year,uint8_t month,uint8_t day,uint8_t *str);
uint8_t Is_Leap_Year(uint16_t year);





#endif 






