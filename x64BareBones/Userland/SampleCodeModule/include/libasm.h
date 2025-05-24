#ifndef LIBASM_H
#define LIBASM_H

#include <stdint.h>

void syscall(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx);

#endif