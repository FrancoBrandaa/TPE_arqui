


#include<soundDriver.h>


void playTone(unsigned int frequency) {
    unsigned int divisor;
    unsigned char tmp;
    /* 1) Calcular divisor para el PIT: 1193180 Hz / frecuencia deseada */
    divisor = 1193180 / frequency;
    /* 2) Configurar canal 2 del PIT en modo “square wave” (modo 3), 16 bits */
    outb(0x43, 0xB6);                          // 0xB6 = 1011 0110b: canal 2, modo 3, acceso por bytes (lo primero el LSB)
    outb(0x42, (unsigned char)(divisor & 0xFF));       // LSB del divisor
    outb(0x42, (unsigned char)((divisor >> 8) & 0xFF)); // MSB del divisor
    /* 3) Habilitar speaker: leer 0x61, ponerle bits 0 y 1 a 1 */
    tmp = inb(0x61);
    outb(0x61, tmp | 0x03); // pone un 1 los dos bits bajos
}

void stopSound(void) {
    unsigned char tmp;
    /* Deshabilitar speaker: leer 0x61 y limpiar bits 0 y 1 */
    tmp = inb(0x61);
    outb(0x61, tmp & 0xFC); // Limpiar los dos bits bajos
}
