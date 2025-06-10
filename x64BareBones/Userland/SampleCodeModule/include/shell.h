#ifndef SHELL_H
#define SHELL_H

#include <libc.h>

/**
 * @brief Initializes and starts the main shell interface.
 * 
 * This function sets up the screen, displays the welcome message,
 * and calls the main shell loop. It serves as the entry point
 * for the shell system.
 */
void shell(void);

/**
 * @brief Main shell loop that handles user input and command execution.
 * 
 * This function runs the interactive shell loop, displaying the prompt,
 * reading user input character by character, handling backspace,
 * and executing commands when Enter is pressed. It continues until
 * the exit flag is set.
 */
void initializeShell(void);

#endif // SHELL_H