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

/**
 * @brief Timer interrupt handler called approximately every 55ms.
 * 
 */
void timer_handler();

/**
 * @brief Gets the current system tick count.
 * 
 * @return int Current tick count.
 */
int ticks_count();

/**
 * @brief Gets the number of seconds since system startup.
 * 
 * @return int Number of seconds since startup.
 */
int seconds_count();

/**
 * @brief Gets the current system date and time.
 * 
 * @return dateStruct* Pointer to structure containing current date/time.
 */
dateStruct * get_time();

/**
 * @brief Suspends execution for at least the specified number of microseconds.
 * 
 * @param microseconds Number of microseconds to sleep.
 */
void sleep(int microseconds);

/**
 * @brief Suspends execution for the specified number of seconds.
 * 
 * @param seconds Number of seconds to sleep.
 */
void sleepSeconds(int seconds);

#endif