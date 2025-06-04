#include <syscallDispatcher.h>

extern uint64_t * getRegisters();

#define SYSNUM_READ 0
#define SYSNUM_WRITE 1
#define SYSNUM_GET_DATE 2
#define SYSNUM_SHOW_REGISTERS 3
#define SYSNUM_SET_CURSOR 5
#define SYSNUM_DRAW_RECTANGLE 6
#define SYSNUM_SET_FONT_COLOR 7
#define SYSNUM_SET_BACKGROUND_FONT_COLOR 10
#define SYSNUM_SET_ZOOM 13
#define SYSNUM_SLEEP 32
#define SYSNUM_GET_CURSOR 33
#define SYSNUM_CLEAN_SCREEN 30
#define SYSNUM_PUT_PIXEL 19
#define SYSNUM_PLAY_TONE 20
#define SYSNUM_STOP_SOUND 21
#define SYSNUM_SPECIAL_KEYS 23
#define SYSNUM_VBE_INFO 9

void sys_write(FDS fd, const char *buf, size_t count) 
{
    if(fd == STDOUT || fd == STDERR) 
        vd_printstr(fd, buf, count);
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
        vd_setCursor(x, y);
    } else if (rax == SYSNUM_SET_FONT_COLOR) {
        uint32_t hexColor = va_arg(args, uint32_t);
        vd_setFontColor(hexColor);
    }  else if (rax == SYSNUM_SET_BACKGROUND_FONT_COLOR) {
        uint32_t hexColor = va_arg(args, uint32_t);
        vd_SetBackgroundColor(hexColor);
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
        sleep(seconds);  
    } else if (rax == SYSNUM_SHOW_REGISTERS) {
        uint64_t * regs = getRegisters();
        showRegisters(regs);
        ret = 0;
    }else if (rax == SYSNUM_GET_CURSOR) {
        int* x = va_arg(args, int*);
        int* y = va_arg(args, int*);
        getCursorPos(x, y);
        ret = 0;
    }else if (rax == SYSNUM_SET_ZOOM) {
        int new_zoom = va_arg(args, int);
        vd_setZoom(new_zoom);
        ret = 0;
    }else if (rax == SYSNUM_CLEAN_SCREEN) {
        vd_cleanScreen();
        ret = 0;
    }else if(rax == SYSNUM_PUT_PIXEL){
        uint32_t hexColor = va_arg(args, uint32_t);
        uint64_t x = va_arg(args, uint64_t);
        uint64_t y = va_arg(args, uint64_t);
        putPixel(hexColor,  x,  y);
        ret = 0;
    }else if (rax == SYSNUM_PLAY_TONE) {
        uint64_t frecuency = va_arg(args, uint64_t);
        playTone(frecuency);
    } else if (rax == SYSNUM_STOP_SOUND) {
        stopSound();
     }  //else if (rax == SYSNUM_SPECIAL_KEYS) {
    //     // Ahora la syscall recibe:
    //     //    1) keyCode (int)  → código ASCII (0..127)
    //     // Retorna 0 (liberada) o 1 (presionada)

    //     int keyCode = (int) va_arg(args, uint64_t);

    //     if (keyCode >= 0 && keyCode < MAX_KEY_CODES) {
    //         ret = (uint64_t) keyStates[keyCode];
    //     } else {
    //         ret = 0;  // Si el keyCode está fuera de rango, devolvemos "liberada"
    //     }
    // }
    va_end(args);
    return ret;
}