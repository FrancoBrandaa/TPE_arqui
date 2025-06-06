// keyboardDriver.c

#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <videoDriver.h>

#define NOT_KEY       -2
#define BUFFER_SIZE   256
#define KEY_ESC       27

// Estado de Shift (0 = liberado; 1 = presionado)
static int isShiftPressed = 0;

// Buffer circular para encolar teclas (ASCII y ESC)
static int  keyBuffer[BUFFER_SIZE];
static int  bufferHead = 0;
static int  bufferTail = 0;

// Array que guarda, para cada código ASCII (0..127), si la tecla está presionada (1) o liberada (0)
static char keyStates[128] = {0};

/**
 * @brief  Mapa de scancode → ASCII sin Shift.
 *         Solo se definen los valores útiles (0..127). El resto queda en 0.
 */
char scancode_to_ascii[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0,  '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',  0,   '*',  0,   ' ',
    // (los índices 58..127 los dejamos en 0, a menos que necesites más símbolos)
};

/**
 * @brief  Mapa de scancode → ASCII **con** Shift (para mayúsculas, signos, etc.)
 */
char shift_ascii[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,   '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',  0,   '*',  0,   ' ',
    // (los índices 58..127 los dejamos en 0 o completarlos según tu mapa de teclas)
};

// Declaración externa que devuelve el scancode actual desde la interrupción
extern unsigned char returnKey();


/**
 * @brief  Rutina de interrupción para el teclado.
 *         - Detecta Shift (presionado/liberado).
 *         - Detecta ESC (solo encolando el ASCII 27 en press).
 *         - Detecta press/release de teclas ASCII estándar:
 *           — En press: marca keyStates[ascii] = 1 y encola el ASCII en el buffer.
 *           — En release: marca keyStates[ascii] = 0.
 *
 *         NO se procesa nada de flechas en esta versión.
 */
void keyboard_handler() {
    unsigned char scancode = returnKey();

    // 1) Detectar Shift presionado o liberado (scancode 42 o 54, ó 170 ó 182 en release)
    if ((scancode & 0x7F) == 42 || (scancode & 0x7F) == 54) {
        if (!(scancode & 0x80)) {
            // Shift presionado (make 42 ó 54, sin bit7)
            isShiftPressed = 1;
        } else {
            // Shift liberado (scancode >= 0x80 con make 42 ó 54)
            isShiftPressed = 0;
        }
        return;
    }

    // 2) Detectar ESC (scancode 0x01 en press; 0x81 en release)
    if ((scancode & 0x7F) == 0x01) {
        if (!(scancode & 0x80)) {
            // ESC presionado → encolar ASCII 27
            int next = (bufferHead + 1) % BUFFER_SIZE;
            if (next != bufferTail) {
                keyBuffer[bufferHead] = KEY_ESC;
                bufferHead = next;
            }
        }
        // Si scancode tiene bit7 (0x81), es release; no hacemos nada especial.
        return;
    }

    // 3) Si es code >= 0x80: significa “liberación” de una tecla
    if (scancode & 0x80) {
        unsigned char make = scancode & 0x7F;  // scancode sin el bit de release
        if (make < 128) {
            // Determinar el ASCII sin importar Shift (en release no importa la mayúscula)
            char ascii = scancode_to_ascii[make];
            if (ascii != 0 && (unsigned char)ascii < 128) {
                keyStates[(unsigned char)ascii] = 0;
            }
        }
        return;
    }

    // 4) Si llegamos acá, es un scancode < 0x80 → “press” de tecla estándar
    if (scancode < 0x80) {
        // Obtener ASCII según si Shift está presionado o no
        char ascii = isShiftPressed ? shift_ascii[scancode] : scancode_to_ascii[scancode];
        if (ascii != 0 && (unsigned char)ascii < 128) {
            // Marcar “presionada” para ese ASCII
            keyStates[(unsigned char)ascii] = 1;

            // Encolar el ASCII en el buffer (para getChar())
            int next = (bufferHead + 1) % BUFFER_SIZE;
            if (next != bufferTail) {
                keyBuffer[bufferHead] = (int)ascii;
                bufferHead = next;
            }
        }
    }
}


/**
 * @brief  Comprueba si hay alguna tecla en el buffer (no bloqueante).
 * @return 1 si hay al menos una tecla en el buffer, 0 si está vacío.
 */
char hasNextKey() {
    return (bufferHead != bufferTail);
}

/**
 * @brief  Saca el siguiente valor del buffer (ASCII o ESC).
 * @return Código ASCII (o 27 para Escape). Si el buffer está vacío, devuelve NOT_KEY (-2).
 */
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

/**
 * @brief  Función pública que devuelve 1 si la tecla ASCII keyCode está presionada,
 *         0 si está liberada o si keyCode está fuera de rango [0..127].
 * @param  keyCode Código ASCII de la tecla a consultar.
 * @return 1 (presionada) o 0 (liberada/o fuera de rango).
 */
char isKeyPressed(int keyCode) {
    if (keyCode < 0 || keyCode >= 128) {
        return 0;
    }
    return keyStates[keyCode];
}
