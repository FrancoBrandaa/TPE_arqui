#include <syscallDispatcher.h>
#include <videoDriver.h>
#include <time.h>
#include <lib.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE 6
static void zero_division();
static void invalid_opcode();

void waitToReturn() {
    char c;
    do {
        _hlt();
        sysRead(STDIN, &c, 1);
    } while (c != '\n');
}

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
    {
        printStr(STDERR, "EXCEPCION: Instruccion invalida detectada!\n", 44);
		zero_division();
    }
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
  setCursor(0,0);
  cleanScreen(); 
  sysWrite(STDERR, msg, strLen(msg));
  showRegisters();
  sysWrite(STDERR, "\n\tPress enter to return to the shell\n", 36);
  swapBuffers();
}