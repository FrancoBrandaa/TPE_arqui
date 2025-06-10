#ifndef DATE_H
#define DATE_H

// Returns current seconds from CMOS (RTC)
char get_secs();

// Returns current minutes from CMOS (RTC)
char get_mins();

// Returns current hour from CMOS (RTC)
char get_hours();

// Returns current day of month from CMOS (RTC)
char get_day();

// Returns current month from CMOS (RTC)
char get_month();

// Returns current year from CMOS (RTC)
char get_year();



#endif // DATE_H
