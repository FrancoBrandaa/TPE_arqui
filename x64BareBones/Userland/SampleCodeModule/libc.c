#include "./include/libc.h"
#include <libasm.h>
typedef enum{
    STDIN = 0,
    STDOUT,
    STDERR,
} FDS;
#define READ 0
#define WRITE 1
#define SET_CURSOR 5
#define SET_FONT_COLOR 7
#define SET_BACKGROUND_FONT_COLOR 10
#define DRAW_RECTANGLE 6

void setFontColor(uint32_t hexColor) {
	sys_call(SET_FONT_COLOR, hexColor, 0, 0, 0);
}
void setBackGroundColor(uint32_t hexColor) {
    sys_call(SET_BACKGROUND_FONT_COLOR, hexColor, 0, 0, 0);
}


void nprint(const char * buf, uint64_t lenght) {
	sys_call(WRITE, STDOUT, (uint64_t) buf, (uint64_t) lenght, 0);
}

void drawRectangle(Point topLeft, Point downRigth, uint32_t color){
    sys_call(DRAW_RECTANGLE, (uint64_t)&topLeft, (uint64_t)&downRigth, (uint64_t)color, 0);
}

void print(const char * buf) {
    nprint(buf, strlen(buf));
}

int scan(char * buf, uint32_t count) {
    return sys_call((uint64_t) READ,(uint64_t) STDIN,(uint64_t) buf,(uint64_t) count, 0);
}

int itoa(uint64_t value, char * buffer, int base, int n) {
    char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
    } while (value /= base);

	// Terminate string in buffer.
    n -= digits;
    while (n > 0) {
        *p++ = '0';
        n--;
    }

    *p = 0x00;
    

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

char getChar() {
    char c;
    scan(&c, 1);
    return c;
}

void putChar(char c) {
    char buf[2] = {c, '\0'};
    print(buf);
}

int strlen(const char * str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void setCursor(uint32_t x, uint32_t y) {
    sys_call(SET_CURSOR, x, y, 0, 0);
}



void setCharCursor(uint32_t x, uint32_t y) {
    setCursor(x*BASE_CHAR_WIDTH, y*BASE_CHAR_HEIGHT);
}

char* strNCpy(const char *src, char *dest, int n) {
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}



