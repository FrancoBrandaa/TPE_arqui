#include <commands.h>
#include <stdint.h>
#include <stddef.h>
#include <libc.h>


int strcmp(const char* s1, const char* s2);
void printString(const char* str);
int strncmp(const char* s1, const char* s2, size_t n);
void runCommand(const char *input) {
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y_exec); 
    if (strcmp(input, "help") == 0) {
        print("Comandos disponibles:\n");
        print(" - help\n");
        print(" - clear\n");
        print(" - echo <mensaje>\n");
    } else if (strcmp(input, "clear") == 0) {
        cleanScreen();
    } else if (strncmp(input, "echo ", 5) == 0) {
        print(input + 5);
        putChar('\n');
    } else {
        print("Comando no reconocido\n");
    }
}


//hacen falta si ya incluyo en string.h
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}


int strncmp(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    return n ? *(unsigned char*)s1 - *(unsigned char*)s2 : 0;
}
