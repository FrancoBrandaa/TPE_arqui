



#include<soundDriver.h>
#include <time.h>


void playTone(int frequency, int duration_ms) 
{
    unsigned int divisor;
    unsigned char tmp;

    divisor = 1193180 / frequency;

    outb(0x43, 0xB6);               
    outb(0x42, (unsigned char)(divisor & 0xFF));       // LSB del divisor
    outb(0x42, (unsigned char)((divisor >> 8) & 0xFF)); // MSB del divisor

    tmp = inb(0x61);
    outb(0x61, tmp | 0x03); // pone 1 en los dos bits bajos

    sleep(duration_ms * 1000);

    tmp = inb(0x61);
    outb(0x61, tmp & 0xFC); // limpiar los dos bits bajos
}

