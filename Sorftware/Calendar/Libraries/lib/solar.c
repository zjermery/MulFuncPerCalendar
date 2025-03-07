#include "solar.h"
#include "math.h"

// 定义节气日期的常量
#define PI 3.14159265358979323846

// 计算天数
int calculate_day_of_year(int month, int day, int year) {
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[1] = 29; // 闰年
    }
    
    int day_of_year = 0;
    for (int i = 0; i < month - 1; i++) {
        day_of_year += days_in_month[i];
    }
    day_of_year += day;
    return day_of_year;
}

// 计算太阳的黄经
double calculate_sun_longitude(int day_of_year) {
    double T = (day_of_year - 1) / 365.25; // 年份的时间变量
    double L0 = 280.46646 + 36000.76983 * T + 0.0003032 * T * T; // 太阳的平均黄经
    L0 = fmod(L0, 360); // 保证黄经在0到360度之间
    return L0;
}

// 判断节气
uint8_t get_solar_term(double longitude) {
    if (longitude >= 0 && longitude < 15)       return 1;
    if (longitude >= 15 && longitude < 30)      return 2;
    if (longitude >= 30 && longitude < 45)      return 3;
    if (longitude >= 45 && longitude < 60)      return 4;
    if (longitude >= 60 && longitude < 75)      return 5;
    if (longitude >= 75 && longitude < 90)      return 6;
    if (longitude >= 90 && longitude < 105)     return 7;
    if (longitude >= 105 && longitude < 120)    return 8;
    if (longitude >= 120 && longitude < 135)    return 9;
    if (longitude >= 135 && longitude < 150)    return 10;
    if (longitude >= 150 && longitude < 165)    return 11;
    if (longitude >= 165 && longitude < 180)    return 12;
    if (longitude >= 180 && longitude < 195)    return 13;
    if (longitude >= 195 && longitude < 210)    return 14;
    if (longitude >= 210 && longitude < 225)    return 15;
    if (longitude >= 225 && longitude < 240)    return 16;
    if (longitude >= 240 && longitude < 255)    return 17;
    if (longitude >= 255 && longitude < 270)    return 18;
    if (longitude >= 270 && longitude < 285)    return 19;
    if (longitude >= 285 && longitude < 300)    return 20;
    if (longitude >= 300 && longitude < 315)    return 21;
    if (longitude >= 315 && longitude < 330)    return 22;
    if (longitude >= 330 && longitude < 345)    return 23;
    return 24;
}

uint8_t get_day_of_solar(int month, int day, int year)
{
    int day_of_year = calculate_day_of_year(month, day, year);
    double longitude = calculate_sun_longitude(day_of_year);
    return get_solar_term(longitude);
}


