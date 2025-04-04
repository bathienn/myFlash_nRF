#include "dateTime.h"

void DateTime_Set(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day,
    uint8_t month, uint16_t year, DateTime_t *dt){
    dt->day = day;
    dt->hour = hour;
    dt->minute = minute;
    dt->month = month;
    dt->second = second;
    dt->year = year;
}

int leap_year(uint16_t year){
    if ((year % 4 ==0 && year % 100 != 0) || year % 400 == 0) return 1;
    else return 0;
}

uint8_t day_of_month(uint8_t month, uint16_t year){
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 ) return 31;
    else
    {
        if (leap_year(year) == 1) return 29;
        else return 28;
    }
}

DateTime_t DateTime_Cal(uint32_t time, DateTime_t dt){
    uint32_t second, minute, hour;
    uint16_t day, month, year;
    second = dt.second;
    minute = dt.minute;
    hour = dt.hour;
    day = dt.day;
    month = dt.month;
    year = dt.year;
    second += time;

    while(second >= 60){
        second -= 60;
        minute++;
    }

    while(minute >= 60){
        minute -= 60;
        hour ++;
    }

    while(hour >= 24){
        hour -= 24;
        day ++;

        while(day > day_of_month(month, year)){
            day -= day_of_month(month,year);
            month ++;

            if (month > 12){
                month -= 12;
                year ++;
            }
        }
    }
    DateTime_t result;
    result.day = day;   
    result.hour = hour;
    result.minute = minute;
    result.month = month;
    result.second = second;
    result.year = year;
    return result;
}