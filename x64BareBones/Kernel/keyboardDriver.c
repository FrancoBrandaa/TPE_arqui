#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <videoDriver.h>

static int isShiftPressed = 0;

#define CHAR_BUFFER_DIM 64
#define NOT_KEY -2
#define BUFFER_SIZE 256

static char charBuffer[CHAR_BUFFER_DIM] = {0};
static char charsAtBuffer=0;
static char getterIndex=0;

static char keyBuffer[BUFFER_SIZE];
static int bufferHead = 0;
static int bufferTail = 0;

char scancode_to_ascii[128] = 
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', // etc.
    // Completa segun tu necesidad
};

char shift_ascii[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

extern unsigned char returnKey();
void keyboard_handler() {
    unsigned char scancode = returnKey();
    
    // Detectar Shift presionado
    if (scancode == 42 || scancode == 54) {
        isShiftPressed = 1;
        return;
    } else if (scancode == 170 || scancode == 182) {
        isShiftPressed = 0;
        return;
    }

    // Solo procesar scancodes válidos
    if (scancode < 128) {
        char ascii = isShiftPressed ? shift_ascii[scancode] : scancode_to_ascii[scancode];
        
        if (ascii != 0) {
            int next = (bufferHead + 1) % BUFFER_SIZE;
            if (next != bufferTail) {
                keyBuffer[bufferHead] = ascii;
                bufferHead = next;
                putChar(ascii);  // Solo para debug
            }
        }
    }
}

char getChar() {
    // Espera activa hasta que haya una tecla
    while (bufferHead == bufferTail);  // bloqueante

    char c = keyBuffer[bufferTail];
    bufferTail = (bufferTail + 1) % BUFFER_SIZE;
    return c;
}

char hasNextKey() {
    return charsAtBuffer > 0;
}

int nextKey() {
    int ret;
    if (!hasNextKey()) {
        return NOT_KEY;
    }
    charsAtBuffer--;
    ret = charBuffer[getterIndex];
    getterIndex++;
    getterIndex = getterIndex % CHAR_BUFFER_DIM;
    return ret;
}