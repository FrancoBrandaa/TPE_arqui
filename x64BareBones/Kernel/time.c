#include <time.h>
#include <naiveConsole.h>

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

dateStruct * get_time () {
    date.sec = get_secs();
    date.min = get_mins();
    date.hour = get_hours(); // Ajuste de huso horario
    date.day = get_day();
    date.month = get_month();
    date.year = get_year();
    return &date;
}

