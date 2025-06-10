#include <time.h>
#include <naiveConsole.h>
#include <stdint.h>

#define TIMEZONE_OFFSET_HOURS -3

static unsigned long ticks = 0;

//Cada vez que interrumpe cada 55mS entra aca y suma un tick
void timer_handler() 
{
	ticks++;
}

int ticks_count() {
	return ticks;
}

//Cada 18 ticks es 1 segundo entonces ticks / 18 te da la cantidad de segundos transcurridos
int seconds_count() {
	return ticks / 18;
}

static dateStruct date;

dateStruct * get_time () 
{
    date.sec = get_secs();
    date.min = get_mins();
    date.hour = get_hours() + TIMEZONE_OFFSET_HOURS; // Ajuste de huso horario

    if (date.hour < 0) 
        date.hour += 24; // Ajuste para horas negativas

    date.day = get_day();
    date.month = get_month();
    date.year = get_year();
    return &date;
}

// Sleep for at least the given number of microseconds
void sleep(int microseconds) {
    unsigned long start = ticks;
    uint64_t ticks_to_wait = ((uint64_t)microseconds * 18 + 999999) / 1000000;
    if (ticks_to_wait == 0) ticks_to_wait = 1;
    while (ticks - start < ticks_to_wait) {
        _hlt();
    }
}

void sleepSeconds(int seconds) {
    if (seconds <= 0) return;
    unsigned long start = ticks;
    uint64_t ticks_to_wait = seconds * 18;
    while (ticks - start < ticks_to_wait) {
        _hlt();
    }
}