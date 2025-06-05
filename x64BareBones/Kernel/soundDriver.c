



#include<soundDriver.h>
#include <time.h>


void playTone(int frequency, int duration_ms) {
    unsigned int divisor;
    unsigned char tmp;
    unsigned long start_ticks;
    unsigned long ticks_to_wait;

    /* 1) Calcular divisor para el PIT: 1193180 Hz / frecuencia deseada */
    divisor = 1193180 / frequency;

    /* 2) Configurar canal 2 del PIT en modo “square wave” (modo 3), 16 bits */
    outb(0x43, 0xB6);                          // 0xB6 = 1011 0110b: canal 2, modo 3, acceso por bytes (LSB first)
    outb(0x42, (unsigned char)(divisor & 0xFF));       // LSB del divisor
    outb(0x42, (unsigned char)((divisor >> 8) & 0xFF)); // MSB del divisor

    /* 3) Habilitar speaker: leer 0x61, ponerle bits 0 y 1 a 1 */
    tmp = inb(0x61);
    outb(0x61, tmp | 0x03); // pone 1 en los dos bits bajos

    /* 4) Calcular cantidad de ticks a esperar */
    sleep(duration_ms * 1000);

    /* 6) Deshabilitar speaker: leer 0x61 y limpiar bits 0 y 1 */
    tmp = inb(0x61);
    outb(0x61, tmp & 0xFC); // limpiar los dos bits bajos
}

