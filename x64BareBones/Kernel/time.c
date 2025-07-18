#include <time.h>
#include <stdint.h>
#include <interrupts.h>

static unsigned long ticks = 0;
static dateStruct date;

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

dateStruct * get_time () 
{
    date.sec = get_secs();
    date.min = get_mins();
    date.hour = get_hours();

    date.day = get_day();
    date.month = get_month();
    date.year = get_year();
    
    return &date;
}

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