
#include <stdint.h>

void int_write(uint64_t fd, const char *buf, uint64_t len) {
	volatile char* video = (char*) 0xB8000;  //creo un puntero a la direccion del inicio de la pantalla (izq_superior)
    int i = 0;
	char color;
	if(fd ==1){
		color = 0xF2;
	}else{
		color = 0xFC; //color rojo
	}
    while (i < len) {
        video[i * 2] = buf[i];      // carácter
        video[i * 2 + 1] = color ;        // atributo: fondo blanco (F), texto verde (2)
        i++;
    }
    // Bucle infinito para que no termine
	while (1);
}