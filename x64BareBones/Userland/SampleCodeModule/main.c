#include <stdint.h>

void putChar(char c);

int main() {
    const char* mensaje = "¡Hola desde userland por syscall!\n";
    for (int i = 0; mensaje[i] != 0; i++) {
        putChar(mensaje[i]);
    }

    while (1);  // no terminar
    return 0;
}

void putChar(char c) {
    __asm__ __volatile__ (
        "movq $1, %%rax\n"       // syscall code 1: putChar
        "movq %0, %%rdi\n"       // argumento: char
        "int $0x80\n"
        :
        : "r"((uint64_t)c)
        : "rax", "rdi"
    );
}
