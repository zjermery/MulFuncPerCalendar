#ifndef __DATE_H__
#define __DATE_H__

#include "stm32f10x.h"

typedef struct  
{
	int tm_sec;           // ��
	int tm_min;           // ��
	int tm_hour;		  // ʱ
	int tm_mday;		  // ��
	int tm_mon;			  // ��
	int tm_year;          // ��
	int tm_wday;          // ��
}rtc_time;
    
void GregorianDay(rtc_time * tm);
uint32_t mktimev(rtc_time *tm);
void to_tm(uint32_t tim,  rtc_time * tm);

#endif
