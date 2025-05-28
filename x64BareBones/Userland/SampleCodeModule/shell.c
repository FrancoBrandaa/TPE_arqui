#include <stdint.h>
#include <shell.h>
#include "./include/libc.h"
#include <libasm.h>
#include <commands.h>

#define COMMAND_DIM ((BASE_DIM_CHAR_X - 4) * 2) // maximo tamaño de comando, sacando margenes [EN CHARS BASE]
#define PRINT_PROMPT()           \
    do {                         \
        setFontColor(white);     \
        print("> ");             \
        setFontColor(actualColor); \
    } while (0)
    
static char exit = 0;
static int command_size = 0;
static char command[COMMAND_DIM];
static void initializeShell();

static uint32_t color[] = {blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple, white};
static int colorIndex = 0;
static int actualColor = green;
static int actualBackgroundFont = black;

static int cursorY = 2 * CHAR_HEIGHT;

int getCursorY()
{
    return cursorY;
}

void incCursorY(int n) 
{
    cursorY += n * CHAR_HEIGHT;
}

static void displayPrompt(void)
{
    // cleanScreen();
    // setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);

    setCursor(COMMAND_LINE_X, 2 * CHAR_HEIGHT); // Start at the top with a small margin
    cursorY = 2 * CHAR_HEIGHT;            // Update the global cursor position

    PRINT_PROMPT();
}

static void cleanCommandLine(void)
{
    // Get the current cursor position to clear just that line
    int currentY = getCursorY(); // You may need to implement this function
    drawRectangle((Point){COMMAND_LINE_X + 2 * CHAR_WIDTH, currentY},
                  (Point){DIM_X, currentY + CHAR_HEIGHT},
                  black);
}

// static void cleanCommandLine(void)
// {
//     drawRectangle((Point){COMMAND_LINE_X + 2 * CHAR_WIDTH, COMMAND_LINE_Y},
//                   (Point){DIM_X, COMMAND_LINE_Y + CHAR_HEIGHT},
//                   black);
// }

void cleanScreen()
{
    drawRectangle((Point){0, 0}, (Point){DIM_X, DIM_Y}, black);
    cursorY = 2 * CHAR_HEIGHT;          // Reset the global cursor position
    setCursor(COMMAND_LINE_X, cursorY); // Reset cursor position for the command line
}

void shell()
{
    cleanScreen();
    setCursor(COMMAND_LINE_X + 2 * CHAR_WIDTH, COMMAND_LINE_Y - 4 * CHAR_HEIGHT);

    initializeShell();
}

void initializeShell()
{
    cursorY = 2 * CHAR_HEIGHT; // Start at the top with a small margin
    setCursor(COMMAND_LINE_X, cursorY);
    PRINT_PROMPT();

    int index = 0;
    while (!exit)
    {
        index = 0;
        while (1)
        {
            char c = getChar();

            if (c != NOT_KEY)
            {
                if (c == '\n')
                { // Enter
                    command[index] = '\0';
                    print("\n");

                    // Execute the command
                    int num_comando = runCommand(command);

                    if (num_comando == 2)// "clear"
                    {
                        PRINT_PROMPT();
                        break; // If the command was "clear", we dont want to display a newline
                    }
                    

                    // Move to the next line for the new prompt
                    cursorY += 2 * CHAR_HEIGHT; // Give some space between commands

                    if (cursorY >= DIM_Y - 3 * CHAR_HEIGHT)
                        cleanScreen();

                    PRINT_PROMPT();
                    break;
                }
                else if (c == '\b')
                { // Backspace
                    if (index > 0)
                    {
                        index--;
                        command[index] = ' ';
                        // Limpia el área del último carácter borrado
                        int charX = COMMAND_LINE_X + 2 * CHAR_WIDTH + index * CHAR_WIDTH;
                        int charY = cursorY;
                        drawRectangle((Point){charX, charY}, (Point){charX + CHAR_WIDTH, charY + CHAR_HEIGHT}, black);
                        setCursor(COMMAND_LINE_X + 2 * CHAR_WIDTH, cursorY);
                        nprint(command, index);
                    }
                }
                else if (index < COMMAND_DIM - 1)
                {
                    command[index++] = c;
                    setCursor(COMMAND_LINE_X + 2 * CHAR_WIDTH, cursorY);
                    nprint(command, index);
                }
                else
                {
                    setCursor(COMMAND_LINE_X + 2 * CHAR_WIDTH, cursorY);
                    print("Comando demasiado largo\n");
                    cursorY += CHAR_HEIGHT;
                    break;
                }
            }
        }
    }
}





// void initializeShell() {
//     displayPrompt();
//     int index = 0;
//     while (!exit) {
//         index = 0;
//         while (1) {
//             // _hlt(); //llamo a hlt la cual no permite que continue la ejecucion hasta que llega una interrupcion
//             char c = getChar();  // asumimos que esta syscall ya está conectad

//             if(c != NOT_KEY){
//             if (index == 0) {
//                 displayPrompt();
//             }
//             if (c == '\n') {  // Enter
//                 command[index] = '\0';
//                 print("\n");
//                 // Ejecutar el comando
//                 runCommand(command);
//                 break;
//             } else if (c == '\b') {  // Backspace
//                 if (index > 0) {
//                     index--;
//                     command[index] = ' ';
//                     cleanCommandLine();
//                     setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y);
//                     nprint(command, index);
//                 }
//             } else if (index < COMMAND_DIM - 1) {
//                 command[index++] = c;
//                 setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y);
//                 nprint(command, index);
//             }else{
//                 setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y);
//                 print("Comando demasiado largo\n");
//                 break;
//             } //que hago si se pasa de la dim??
//             }
//         }
//     }
// }
