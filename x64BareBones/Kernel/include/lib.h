#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <videoDriver.h>
#include <interrupts.h>

char* strNCpy(const char *src, char *dest, int n);
void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void showRegisters();
int strLen(char * str);


#endif