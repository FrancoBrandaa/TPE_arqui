#include "commands.h"
#include "videoDriver.h"
#include <stdint.h>
#include <naiveConsole.h>
#include <stddef.h>
#include <string.h>
int strcmp(const char* s1, const char* s2);
void printString(const char* str);
int strncmp(const char* s1, const char* s2, size_t n);
void runCommand(const char *input) {
    if (strcmp(input, "help") == 0) {
        printString("Comandos disponibles:\n");
        printString(" - help\n");
        printString(" - clear\n");
        printString(" - echo <mensaje>\n");
    } else if (strcmp(input, "clear") == 0) {
        ncClear();
    } else if (strncmp(input, "echo ", 5) == 0) {
        printString(input + 5);
        putChar('\n');
    } else {
        printString("Comando no reconocido\n");
    }
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void printString(const char* str) {
    if (str == 0) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        putChar(str[i]);
    }
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    return n ? *(unsigned char*)s1 - *(unsigned char*)s2 : 0;
}
