#ifndef _SOLAR_H
#define _SOLAR_H

#include "sys.h"

int calculate_day_of_year(int month, int day, int year);
double calculate_sun_longitude(int day_of_year);
uint8_t get_day_of_solar(int month, int day, int year);

#endif






    

