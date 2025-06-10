#include <syscallDispatcher.h>

extern uint64_t * getRegisters();

#define SYSNUM_READ 0
#define SYSNUM_WRITE 1
#define SYSNUM_GET_DATE 2
#define SYSNUM_SHOW_REGISTERS 3
#define SYSNUM_SET_CURSOR 4
#define SYSNUM_PUT_PIXEL 6
#define SYSNUM_CLEAN_SCREEN 7
#define SYSNUM_SET_FONT_COLOR 8
#define SYSNUM_SET_BACKGROUND_FONT_COLOR 9
#define SYSNUM_SET_ZOOM 10
#define SYSNUM_SLEEP 11
#define SYSNUM_PLAY_TONE 12
#define SYSNUM_VBE_INFO 13
#define SYSNUM_IS_KEY_PRESSED 14
#define SYSNUM_SWAP_BUFFERS 15


void sysWrite(FDS fd, const char *buf, size_t count) 
{
    if(fd == STDOUT || fd == STDERR) 
        printStr(fd, buf, count);
    return;
}

uint32_t readChars(char * buf, size_t count) {
    int i = 0, c;
    int end = 0;
    while (i < count && !end) {
        c = nextKey();
        buf[i++] = (char) c;
        end = (c == -2);
    }
    return i - end;         
}

size_t sysRead(FDS fd, char *buf, size_t count) {
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
        ret = sysRead(fd, buf, count);
    } else if (rax == SYSNUM_WRITE) {
        FDS fd = va_arg(args, FDS);
        const char * buf = va_arg(args, const char*);
        uint64_t count = va_arg(args, uint64_t);
        sysWrite(fd, buf, count);
        ret = 0;
    } else if (rax == SYSNUM_SET_CURSOR) {
        int x = (int)va_arg(args, uint64_t);
        int y = (int)va_arg(args, uint64_t);
        setCursor(x, y);
    } else if (rax == SYSNUM_SET_FONT_COLOR) {
        uint32_t hexColor = va_arg(args, uint32_t);
        setFontColor(hexColor);
    }  else if (rax == SYSNUM_SET_BACKGROUND_FONT_COLOR) {
        uint32_t hexColor = va_arg(args, uint32_t);
        SetBackgroundColor(hexColor);
    }else if(rax == SYSNUM_GET_DATE){
        ret = get_time();
    }else if (rax == SYSNUM_SLEEP) {
        int microseconds = va_arg(args, int);
        sleep(microseconds);  
    } else if (rax == SYSNUM_SHOW_REGISTERS) {
        uint64_t * regs = getRegisters();
        showRegisters(regs);
        ret = 0;
    }else if (rax == SYSNUM_SET_ZOOM) {
        int new_zoom = va_arg(args, int);
        setZoom(new_zoom);
        ret = 0;
    }else if (rax == SYSNUM_CLEAN_SCREEN) {
        cleanScreen();
        ret = 0;
    }else if(rax == SYSNUM_PUT_PIXEL){
        uint32_t hexColor = va_arg(args, uint32_t);
        uint64_t x = va_arg(args, uint64_t);
        uint64_t y = va_arg(args, uint64_t);
        putPixel(hexColor,  x,  y);
        ret = 0;
    }else if (rax == SYSNUM_PLAY_TONE) {
        uint64_t frecuency = va_arg(args, uint64_t);
        uint64_t delay = va_arg(args, uint64_t);
        playTone(frecuency,delay);
    }else if(rax == SYSNUM_VBE_INFO){
        neededInfo x = va_arg(args, neededInfo); //puntero a structura
        ret = getVbeInfo(x); //cero si funca bien
    }else if (rax == SYSNUM_IS_KEY_PRESSED) {
        // El usuario pasa un keyCode (ASCII 0..127) y devolvemos 1 si está presionado o 0 si no.
        int keyCode = (int)va_arg(args, uint64_t);
        // Llamamos a la función de teclado que definimos en keyboardDriver.c
        ret = (uint64_t) isKeyPressed(keyCode);
    } else if (rax == SYSNUM_SWAP_BUFFERS) {
        swapBuffers();
        ret = 0;
    }
    va_end(args);
    return ret;
}