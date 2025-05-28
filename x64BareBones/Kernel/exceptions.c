#include <syscallDispatcher.h>
#include <videoDriver.h>
#include <lib.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE 6
static void zero_division();
static void invalid_opcode();

void waitToReturn() {
    char c;
    do {
        _hlt();
        sys_read(STDIN, &c, 1);
    } while (c != '\n');
}

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if (exception == INVALID_OPCODE)
        invalid_opcode();
}

static void zero_division() {
	_sti();
	printError("Invalid division\n");
	waitToReturn();
}

static void invalid_opcode() {
    _sti();
	printError("Invalid opcode\n");
	waitToReturn();
}

void printError(char * msg){
  drawRectangle(&((Point){0,0}), &((Point){DIM_X, DIM_Y}), 0x000000);
  sys_write(STDERR, msg, strLen(msg));
  sys_sleep(5);
  showRegisters();
  sys_sleep(5);
  sys_write(STDERR, "\n\tPress enter to return to the shell\n", 36);
  
}