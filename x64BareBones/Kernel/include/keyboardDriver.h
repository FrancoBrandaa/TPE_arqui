
#include <videoDriver.h>

#define NOT_KEY -2
#define BUFFER_SIZE 256

#define KEY_ESC 27
#define KEY_ARROW_UP    1001
#define KEY_ARROW_DOWN  1002
#define KEY_ARROW_LEFT  1003
#define KEY_ARROW_RIGHT 1004

#define MAX_KEY_CODES 128

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
void keyboard_handler();

/**
 * @brief  Bloqueante: espera hasta que haya un carácter en el buffer y lo devuelve.
 */
char getChar();

/**
 * @brief  Comprueba si hay alguna tecla en el buffer (no bloqueante).
 * @return 1 si hay al menos una tecla en el buffer, 0 si está vacío.
 */
char hasNextKey();

/**
 * @brief  Función pública que devuelve 1 si la tecla ASCII keyCode está presionada,
 *         0 si está liberada o si keyCode está fuera de rango [0..127].
 * @param  keyCode Código ASCII de la tecla a consultar.
 * @return 1 (presionada) o 0 (liberada/o fuera de rango).
 */
int isKeyPressed(int keyCode);