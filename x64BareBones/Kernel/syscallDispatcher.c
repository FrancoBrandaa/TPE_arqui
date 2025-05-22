#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

#define SYSCALL_PUTCHAR 1
#define SYSCALL_GETCHAR 2

void syscallDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx) {
    switch (rax) {
        case SYSCALL_PUTCHAR:
            putChar((char)rdi);  // Usa tu función que imprime caracteres
            break;

        case SYSCALL_GETCHAR:
            *((char*)rdi) = getChar();  // Guarda el char leído en la dirección pasada
            break;

        default:
            // syscall desconocida, no hacer nada por ahora
            break;
    }
}
