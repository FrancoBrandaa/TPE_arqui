#ifndef _TIME_H_
#define _TIME_H_

#include <date.h>

typedef struct dateStruct {
    char sec;
    char min;
    char hour;
    char day;
    char month;
    char year;
} dateStruct;

void timer_handler();
int ticks_count();
int seconds_count();
dateStruct * get_time (); 

#endif
