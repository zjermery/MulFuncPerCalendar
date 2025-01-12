#ifndef __DATE_H__
#define __DATE_H__

#include "stm32f10x.h"

typedef struct  
{
	int tm_sec;           // 秒
	int tm_min;           // 分
	int tm_hour;		  // 时
	int tm_mday;		  // 日
	int tm_mon;			  // 月
	int tm_year;          // 年
	int tm_wday;          // 周
}rtc_time;
    
void GregorianDay(rtc_time * tm);
uint32_t mktimev(rtc_time *tm);
void to_tm(uint32_t tim,  rtc_time * tm);

#endif
