#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdint.h>
#include <libc.h>
#include <shell.h>
#include <pongisGolf.h>

/**
 * @brief Executes a command based on the input string provided
 * 
 * This function parses and executes a command from the given input string.
 * It identifies the command type and calls the appropriate handler function.
 * 
 * @param input Null-terminated string containing the command to execute
 * @return an int number indicating which command was executed, -1 if the command is not recognized.
 */
int runCommand(const char *input);



#endif
