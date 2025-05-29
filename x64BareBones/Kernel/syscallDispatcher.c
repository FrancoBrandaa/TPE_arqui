#include <syscallDispatcher.h>
#include <stdarg.h>
#include <time.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

extern uint64_t * getRegisters();

#define SYSNUM_READ 0
#define SYSNUM_WRITE 1
#define SYSNUM_GET_DATE 2
#define SYSNUM_SHOW_REGISTERS 3
#define SYSNUM_SET_CURSOR 5
#define SYSNUM_DRAW_RECTANGLE 6
#define SYSNUM_SET_FONT_COLOR 7
#define SYSNUM_SET_BACKGROUND_FONT_COLOR 10
#define SYSNUM_SLEEP 32


#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_RED 0xFF0000

uint32_t color = COLOR_WHITE;
uint32_t backgroundColor = COLOR_BLACK;
int cursorX=0, cursorY=0;
uint8_t zoom = 1;

// Se setea el cursor [EN PIXELES]
size_t sys_setCursor(int x, int y) {
    cursorX = x;
    cursorY = y;
    return 1;
}

void setFontColor(uint32_t hexColor) {
    color = hexColor;
}

void sys_setZoom(int new_zoom) {
    zoom = new_zoom;
}
// void getCursor(int *x, int *y) {
//     *x = cursorX;
//     *y = cursorY;
// }

void changeBackgroundColor(uint32_t hexColor) {  
    backgroundColor = hexColor;
}

void sys_write(FDS fd, const char *buf, size_t count) {
    if(fd == STDOUT || fd == STDERR) {
        int i = 0;
        while (i < count) {
            while (i < count && (cursorX+CHAR_WIDTH*zoom) < DIM_X && buf[i] != '\n') {    
                drawChar(buf[i], cursorX, cursorY, (fd==STDOUT)?color:COLOR_RED, backgroundColor, zoom);
                cursorX += CHAR_WIDTH*zoom;
                i++;
            }
            if (buf[i] == '\n' || i < count) {
                cursorX = 0;
                cursorY += CHAR_HEIGHT*zoom;
            }
            i += (buf[i] == '\n');  // si tengo un salto de linea, salteo
        }
    }
    return;
}

//xd
void sys_sleep(int seconds){
    sleep(seconds);
}

uint32_t readChars(char * buf, size_t count) {
    int i = 0, c;
    int end = 0;
    while (i < count && !end) {
        c = nextKey();
        buf[i++] = (char) c;
        end = (c == -2);
    }
    return i - end;             // si end es 1, resto un caracter (que seria el -2)
}


size_t sys_read(FDS fd, char *buf, size_t count) {
    if (fd == STDIN) {
        return readChars(buf, count);
    }
    return 0;
}

void syscallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);
    uint64_t ret;
    if (rax == SYSNUM_READ) {
       uint64_t fd = va_arg(args, uint64_t);
       uint64_t buf = va_arg(args, uint64_t);
        uint64_t count = va_arg(args, uint64_t);
        ret = sys_read(fd, buf, count);
    } else if (rax == SYSNUM_WRITE) {
        FDS fd = va_arg(args, FDS);
        const char * buf = va_arg(args, const char*);
        uint64_t count = va_arg(args, uint64_t);
        sys_write(fd, buf, count);
        ret = 0;
    } else if (rax == SYSNUM_SET_CURSOR) {
        int x = (int)va_arg(args, uint64_t);
        int y = (int)va_arg(args, uint64_t);
        sys_setCursor(x, y);
    } else if (rax == SYSNUM_SET_FONT_COLOR) {
        uint32_t hexColor = va_arg(args, uint32_t);
        setFontColor(hexColor);
    }  else if (rax == SYSNUM_SET_BACKGROUND_FONT_COLOR) {
        uint32_t hexColor = va_arg(args, uint32_t);
        changeBackgroundColor(hexColor);
    }else if(rax == SYSNUM_DRAW_RECTANGLE){
        Point* x = va_arg(args, Point*);
        Point* y = va_arg(args, Point*);
        uint32_t hexColor = va_arg(args, uint32_t);
        drawRectangle(x, y, hexColor); //llamo directo a la fun del driver
        ret = 0;
    }else if(rax == SYSNUM_GET_DATE){
        ret = get_time();
    }else if (rax == SYSNUM_SLEEP) {
        int seconds = va_arg(args, int);
        sys_sleep(seconds);    
    } else if (rax == SYSNUM_SHOW_REGISTERS) {
        uint64_t * regs = getRegisters();
        showRegisters(regs);
        ret = 0;
    }
    va_end(args);
    return ret;
}