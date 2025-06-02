#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <videoDriver.h>

static int isShiftPressed = 0;

//#define CHAR_BUFFER_DIM 64
#define NOT_KEY -2
#define BUFFER_SIZE 256


#define KEY_ESC 27
#define KEY_ARROW_UP    1001
#define KEY_ARROW_DOWN  1002
#define KEY_ARROW_LEFT  1003
#define KEY_ARROW_RIGHT 1004
static int ext_scancode = 0;

// keyBuffer a int para poder encolar flechas y ESC (27)
static int keyBuffer[BUFFER_SIZE];
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

    // 1) Detectar prefijo extendido (0xE0)
    if (scancode == 0xE0) {
        ext_scancode = 1;
        return;
    }
    if (ext_scancode) {
        int code = 0;
        switch (scancode) {
            case 0x48:
                code = KEY_ARROW_UP;
                break;
            case 0x50:
                code = KEY_ARROW_DOWN;
                break;
            case 0x4B:
                code = KEY_ARROW_LEFT;
                break;
            case 0x4D:
                code = KEY_ARROW_RIGHT;
                break;
            default:
                ext_scancode = 0;
                return;  // No era flecha
        }
        ext_scancode = 0;
        // Encolar código de flecha
        int next = (bufferHead + 1) % BUFFER_SIZE;
        if (next != bufferTail) {
            keyBuffer[bufferHead] = code;
            bufferHead = next;
        }
        return;
    }

    // 2) Detectar Shift presionado / liberado
    if ((scancode & 0x7F) == 42 || (scancode & 0x7F) == 54) {
        if (!(scancode & 0x80)) {
            // Shift presionado
            isShiftPressed = 1;
        } else {
            // Shift liberado (scancode | 0x80)
            isShiftPressed = 0;
        }
        return;
    }

    // 3) Detectar Escape presionado
    //    Scancode de Escape = 0x01 (y 0x81 al liberar, pero aquí solo interesa presionar)
    if (scancode == 0x01) {
        // Encolar ASCII 27 (Escape)
        int next = (bufferHead + 1) % BUFFER_SIZE;
        if (next != bufferTail) {
            keyBuffer[bufferHead] = 27;
            bufferHead = next;
        }
        return;
    }

    // 4) Solo procesar scancodes válidos (< 128) y que no sean release
    if (scancode < 0x80) {
        int ascii = isShiftPressed
                    ? shift_ascii[scancode]
                    : scancode_to_ascii[scancode];

        if (ascii != 0) {
            int next = (bufferHead + 1) % BUFFER_SIZE;
            if (next != bufferTail) {
                keyBuffer[bufferHead] = ascii;
                bufferHead = next;
            }
        }
    }
}


char hasNextKey() {
    // Non-blocking check for pending keys
    return (bufferHead != bufferTail);
}

int nextKey() {
    // Return next key or NOT_KEY if buffer is empty
    if (!hasNextKey()) {
        return NOT_KEY;
    }
    char c = keyBuffer[bufferTail];
    bufferTail = (bufferTail + 1) % BUFFER_SIZE;
    return c;
}


// void keyboard_handler() {
//     unsigned char scancode = returnKey();
    
//     // Detectar Shift presionado
//     if (scancode == 42 || scancode == 54) {
//         isShiftPressed = 1;
//         return;
//     } else if (scancode == 170 || scancode == 182) {
//         isShiftPressed = 0;
//         return;
//     }

//     // Solo procesar scancodes válidos
//     if (scancode < 128) {
//         char ascii = isShiftPressed ? shift_ascii[scancode] : scancode_to_ascii[scancode];
        
//         if (ascii != 0) {
//             int next = (bufferHead + 1) % BUFFER_SIZE; //RARO ESTO
//             if (next != bufferTail) {
//                 keyBuffer[bufferHead] = ascii;
//                 bufferHead = next;
//                 //putChar(ascii);  // Solo para debug
//             }
//         }
//     }
// }