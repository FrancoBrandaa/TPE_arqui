#include <stdint.h>
#include <shell.h>
#include "./include/libc.h"
#include <libasm.h>
#include <commands.h>


#define COMMAND_DIM ((BASE_DIM_CHAR_X-4)*2)  // maximo tamaño de comando, sacando margenes [EN CHARS BASE]
static char exit = 0;
static int command_size = 0;
static char command[COMMAND_DIM]; 
static void initializeShell();

static uint32_t color[] = { blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple,white};
static int colorIndex = 0;
static int actualColor = white;
static int actualBackgroundFont=black;

static void displayPrompt(void) {
    cleanScreen();
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
    setFontColor(white);
    print("> ");
    setFontColor(actualColor);
}

static void cleanCommandLine(void) {
    drawRectangle((Point){COMMAND_LINE_X + 2 * CHAR_WIDTH, COMMAND_LINE_Y},
                  (Point){DIM_X, COMMAND_LINE_Y + CHAR_HEIGHT},
                  black);
}

void cleanScreen()
{
    drawRectangle((Point){0, 0}, (Point){DIM_X, DIM_Y}, black);
}
void shell (){
    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y-4*CHAR_HEIGHT);
    initializeShell();
}


void initializeShell() {
    displayPrompt();
    int index = 0;
    while (!exit) {
        index = 0;
        while (1) {
            // _hlt(); //llamo a hlt la cual no permite que continue la ejecucion hasta que llega una interrupcion
            char c = getChar();  // asumimos que esta syscall ya está conectad
            
            if(c != NOT_KEY){
            if (index == 0) {
                displayPrompt();
            }
            if (c == '\n') {  // Enter
                command[index] = '\0';
                print("\n");
                // Ejecutar el comando
                runCommand(command);
                break;
            } else if (c == '\b') {  // Backspace
                if (index > 0) {
                    index--;
                    command[index] = ' ';
                    cleanCommandLine();
                    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y); 
                    nprint(command, index);
                }
            } else if (index < COMMAND_DIM - 1) {
                command[index++] = c;
                setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y); 
                nprint(command, index);
            }else{
                setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y); 
                print("Comando demasiado largo\n");
                break;
            } //que hago si se pasa de la dim??
            }
        }
    }
}


