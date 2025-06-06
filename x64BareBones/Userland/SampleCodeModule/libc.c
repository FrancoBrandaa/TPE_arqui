#include "./include/libc.h"

#define STRING_MAX_LENGTH 1024
#define READ 0
#define WRITE 1
#define GET_DATE 2
#define PRINT_REGISTERS 3
#define SET_CURSOR 5
#define SET_FONT_COLOR 7
#define SET_BACKGROUND_FONT_COLOR 10
#define DRAW_RECTANGLE 6
#define SET_ZOOM 13
#define SLEEP 32
#define CLEAN_SCREEN 30
#define GET_CURSOR 33
#define PUT_PIXEL 19
#define PLAY_TONE 20
#define VBE_GET_INFO 9
#define DRAW_CIRCLE 21
#define IS_KEY_PRESSED 23

// Definición de las variables globales:
uint16_t DIM_X = 0;
uint16_t DIM_Y = 0;

int initScreenSize(void) {
    struct vbeInfo info;               // la estructura donde se guardará el ancho y alto
    int err = getVbeInfo(&info);       // syscall que copia info desde el kernel
    
    if (err != 0) {
        // no se obtuvo bien la info desde el kernel
        return -1;
    }
    // Si llegamos acá, 'info.width' y 'info.height' tienen la info de la VBE.
    DIM_X = info.width;
    DIM_Y = info.height;
    return 0;
}
//le paso un puntero a la estructura a llenar y si me retorna 0 es que lo lleno correctamente
int getVbeInfo(neededInfo info){
    return sys_call(VBE_GET_INFO, info, 0, 0,0);
}

int isKeyPressed (int keyCode) {
    return sys_call(IS_KEY_PRESSED, keyCode, 0, 0, 0);
}

void drawCircle(int centerX, int centerY, int radius, uint32_t color){
    sys_call(DRAW_CIRCLE, centerX, centerY, radius, color);
}
void playTone(uint64_t frequency,uint64_t duration_ms) {
    sys_call(PLAY_TONE, frequency, duration_ms, 0,0 );
}
void setFontColor(uint32_t hexColor)
{
    sys_call(SET_FONT_COLOR, hexColor, 0, 0,0);
}
void setBackGroundColor(uint32_t hexColor)
{
    sys_call(SET_BACKGROUND_FONT_COLOR, hexColor, 0, 0,0);
}

void putPixel(uint32_t color,uint64_t x, uint64_t y){
    sys_call(PUT_PIXEL, color , x, y,0);
}


printRegisters()
{
    sys_call(PRINT_REGISTERS, 0, 0, 0,0);
}

dateStruct *getDate()
{
    dateStruct *date = sys_call(GET_DATE, 0, 0, 0,0);
    return date;
}

void getCursorPos(int *x, int *y)
{
    sys_call(GET_CURSOR, (uint64_t)x, (uint64_t)y, 0,0);
}

void timeToStr(char *buffer)
{
    dateStruct *date = getDate();
    strCpy("dd/mm/yy 00:00:00", buffer);
    char aux[3] = {0x00};
    itoa(date->day, aux, 16, 2);
    strNCpy(aux, buffer, 2);
    itoa(date->month, aux, 16, 2);
    strNCpy(aux, buffer + 3, 2);
    itoa(date->year, aux, 16, 2);
    strNCpy(aux, buffer + 6, 2);
    itoa(date->hour, aux, 16, 2); // LA HORA ESTA CORRIDA 5 HORAS, VER COMO FIXEAR
    strNCpy(aux, buffer + 9, 2);
    itoa(date->min, aux, 16, 2);
    strNCpy(aux, buffer + 12, 2);
    itoa(date->sec, aux, 16, 2);
    strNCpy(aux, buffer + 15, 2);
}

void printDate()
{

    // sizeof("dd/mm/yy 00:00:00") == 18, incluye el '\0'
    char buffer[] = "dd/mm/yy 00:00:00";
    timeToStr(buffer);
    print(buffer);
    putChar('\n'); // ← ¡salto de línea después de la fecha!
}

void nprint(const char *buf, uint64_t lenght)
{
    sys_call(WRITE, STDOUT, (uint64_t)buf, (uint64_t)lenght,0);
}

void drawRectangle(Point topLeft, Point downRigth, uint32_t color)
{
    sys_call(DRAW_RECTANGLE, (uint64_t)&topLeft, (uint64_t)&downRigth, (uint64_t)color, 0);
}

void sleep(uint64_t seconds)
{
    sys_call(SLEEP, seconds, 0,0,0);
}

void print(const char *buf)
{
    nprint(buf, strlen(buf));
}

#include <libc.h>
// … resto de implementaciones …

#include <libc.h>
// … resto de implementaciones …

void printInt(int value) {
    if (value == 0) {
        putChar('0');
        return;
    }
    if (value < 0) {
        putChar('-');
        value = -value;
    }
    char buf[12];
    int i = 0;
    while (value > 0) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }
    while (i--) {
        putChar(buf[i]);
    }
}

int scan(char *buf, uint32_t count)
{
    return sys_call((uint64_t)READ, (uint64_t)STDIN, (uint64_t)buf, (uint64_t)count,0);
}

int itoa(uint64_t value, char *buffer, int base, int n)
{
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    // Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    n -= digits;
    while (n > 0)
    {
        *p++ = '0';
        n--;
    }

    *p = 0x00;

    // Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

char getChar()
{
    char c;
    scan(&c, 1);
    return c;
}

void putChar(char c)
{
    nprint(&c, 1);
}

int strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

void cleanScreen()
{
    sys_call(CLEAN_SCREEN, 0, 0, 0,0);
}

void setCursor(uint32_t x, uint32_t y)
{
    sys_call(SET_CURSOR, x, y, 0,0);
}

void setZoom(int new_zoom)
{
    sys_call(SET_ZOOM, new_zoom, 0, 0,0);
}

void setCharCursor(uint32_t x, uint32_t y)
{
    setCursor(x * BASE_CHAR_WIDTH, y * BASE_CHAR_HEIGHT);
}

void strCpy(const char *source, char *dest)
{
    while ((*dest++ = *source++));
}

char* strNCpy(const char * source, char * dest, int n)
{
    while (n-- && (*dest++ = *source++));
    if (n >= 0)
        *dest = '\0'; // Null-terminate if there's space
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
        n--;
    }
    return n ? *(unsigned char *)s1 - *(unsigned char *)s2 : 0;
}

int atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    // Skip whitespace
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    // Handle sign
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }
    // Convert digits
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return sign * result;
}


