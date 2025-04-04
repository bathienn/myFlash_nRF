#ifndef DATETIME_H
#define DATETIME_H
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t day, month;
    uint8_t hour, minute, second;
    uint16_t year;
} DateTime_t;

// set default date and time
void DateTime_Set(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day,
    uint8_t month, uint16_t year, DateTime_t *dt);

// calculate date and time after time(second)
DateTime_t DateTime_Cal(uint32_t time, DateTime_t dt);

#endif  /*DATETIME_H*/