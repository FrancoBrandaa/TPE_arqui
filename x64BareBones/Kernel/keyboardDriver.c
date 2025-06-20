// keyboardDriver.c

#include <keyboardDriver.h>


// Estado de Shift (0 = liberado; 1 = presionado)
static int isShiftPressed = 0;

// Buffer circular para encolar teclas (ASCII y ESC)
static int  keyBuffer[BUFFER_SIZE];
static int  bufferHead = 0;
static int  bufferTail = 0;

static char keyStates[128] = {0};

/**
 * @brief  Mapa de scancode → ASCII sin Shift.
 *         Solo se definen los valores útiles (0..127). El resto queda en 0.
 */
char scancode_to_ascii[128] = 
{
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0,  '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',  0,   '*',  0,   ' ',
};

/**
 * @brief  Mapa de scancode → ASCII **con** Shift (para mayúsculas, signos, etc.)
 */
char shift_ascii[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,   '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',  0,   '*',  0,   ' ',
};

extern unsigned char returnKey();

void keyboard_handler() 
{
    unsigned char scancode = returnKey();

    if ((scancode & 0x7F) == 42 || (scancode & 0x7F) == 54) {
        if (!(scancode & 0x80)) {
            // Shift presionado
            isShiftPressed = 1;
        } else {
            // Shift liberado
            isShiftPressed = 0;
        }
        return;
    }

    if ((scancode & 0x7F) == 0x01) {
        if (!(scancode & 0x80)) {
            // ESC presionado
            int next = (bufferHead + 1) % BUFFER_SIZE;
            if (next != bufferTail) {
                keyBuffer[bufferHead] = KEY_ESC;
                bufferHead = next;
            }
        }
        return;
    }

    if (scancode & 0x80) {
        unsigned char make = scancode & 0x7F;
        if (make < 128) {
            // Determinar el ASCII sin importar Shift
            char ascii = scancode_to_ascii[make];
            if (ascii != 0 && (unsigned char)ascii < 128) {
                keyStates[(unsigned char)ascii] = 0;
            }
        }
        return;
    }

    if (scancode < 0x80) {
        // Obtiene ASCII en funcion del shift
        char ascii = isShiftPressed ? shift_ascii[scancode] : scancode_to_ascii[scancode];
        if (ascii != 0 && (unsigned char)ascii < 128) 
        {
            // Marcar “presionada” para ese ASCII
            keyStates[(unsigned char)ascii] = 1;

            int next = (bufferHead + 1) % BUFFER_SIZE;
            if (next != bufferTail) {
                keyBuffer[bufferHead] = (int)ascii;
                bufferHead = next;
            }
        }
    }
}

char hasNextKey() {
    return (bufferHead != bufferTail);
}

int nextKey() {
    if (!hasNextKey()) {

        return NOT_KEY;
    }
    int c = keyBuffer[bufferTail];
    bufferTail = (bufferTail + 1) % BUFFER_SIZE;
    return c;
}

/**
 * @brief  Bloqueante: espera hasta que haya un carácter en el buffer y lo devuelve.
 */
char getChar() {
    while (bufferHead == bufferTail) {
        // espera activa
    }
    int c = keyBuffer[bufferTail];
    bufferTail = (bufferTail + 1) % BUFFER_SIZE;
    return (char)c;
}

int isKeyPressed(int keyCode) {
    if (keyCode < 0 || keyCode >= 128) 
    {
        return 0; 
    }
    return keyStates[keyCode];
}
