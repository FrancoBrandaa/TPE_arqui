#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

//==============================================================================
// CONSTANTS
//==============================================================================

/**
 * @brief Value returned by nextKey() when no keys are available.
 */
#define NOT_KEY -2

/**
 * @brief Size of the circular key buffer.
 */
#define BUFFER_SIZE 256

/**
 * @brief ASCII code for the ESC key.
 */
#define KEY_ESC 27

/**
 * @brief Special codes for arrow keys (not yet implemented).
 */
#define KEY_ARROW_UP     1001
#define KEY_ARROW_DOWN   1002
#define KEY_ARROW_LEFT   1003
#define KEY_ARROW_RIGHT  1004

/**
 * @brief Maximum number of ASCII key codes handled (0..127).
 */
#define MAX_KEY_CODES 128

//==============================================================================
// FUNCTIONS
//==============================================================================

/**
 * @brief Main keyboard interrupt handler.
 * 
 * Should be called from the IRQ1 routine. Translates scancodes
 * to ASCII characters and queues valid keys in the buffer.
 */
void keyboard_handler();

/**
 * @brief Checks if there are keys in the buffer (non-blocking).
 * 
 * @return int 1 if keys are available, 0 if buffer is empty.
 */
int hasKeys();

/**
 * @brief Gets the next key from the buffer (non-blocking).
 * 
 * @return int ASCII code of the next key, or NOT_KEY if buffer is empty.
 */
int nextKey();

/**
 * @brief Checks if a specific key is currently pressed.
 * 
 * @param keyCode ASCII code of the key to check.
 * @return int 1 if the key is pressed, 0 if released.
 */
int isKeyPressed(int keyCode);

/**
 * @brief Clears the key buffer.
 * 
 * Removes all pending keys from the circular buffer,
 * effectively resetting the keyboard input queue.
 */
void clearKeyBuffer();

#endif // KEYBOARD_DRIVER_H