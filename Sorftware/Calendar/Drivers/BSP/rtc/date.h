#ifndef __DATE_H__
#define __DATE_H__

#include "stm32f10x.h"

struct rtc_time 
{
	int tm_sec;           // Ãë
	int tm_min;           // ·Ö
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;          // Äê
	int tm_wday;          // ÖÜ
};
    
void GregorianDay(struct rtc_time * tm);
uint32_t mktimev(struct rtc_time *tm);
void to_tm(uint32_t tim, struct rtc_time * tm);

#endif
