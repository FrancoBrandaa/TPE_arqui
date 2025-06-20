#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>
#include <lib.h>
#include <stdarg.h>
#include <interrupts.h>
#include <time.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <soundDriver.h>


uint64_t sysCallDispatcher(uint64_t rax, ...);
void sysWrite(FDS fd, const char *buf, size_t count);
uint32_t readChars(char *buf, size_t count);
size_t sysRead(FDS fd, char *buf, size_t count);

#endif