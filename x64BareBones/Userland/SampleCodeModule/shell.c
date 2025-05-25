#include <stdint.h>
#include <shell.h>
#include "./include/libc.h"
#include <libasm.h>
#include <commands.h>



static char exit = 0;

int zoom_user = 1;


#define CHAR_WIDTH (BASE_CHAR_WIDTH*zoom_user)       // Ancho de un char actual [EN PIXELES]
#define CHAR_HEIGHT (BASE_CHAR_HEIGHT*zoom_user)     // Alto de un char actual [EN PIXELES]

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)      // Ancho de pantalla [EN CHARS ACTUALES]
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)       // Alto de pantalla [EN CHARS ACTUALES]

#define COMMAND_LINE_X (2*BASE_CHAR_WIDTH)            // Pos de x de la linea de comandos [EN PIXELES]
#define COMMAND_LINE_Y (DIM_Y-(2*CHAR_HEIGHT))        // Pos de y de la linea de comandos [EN PIXELES]
#define COMMAND_DIM ((BASE_DIM_CHAR_X-4)*2)  // maximo tamaño de comando, sacando margenes [EN CHARS BASE]

static int command_size = 0;
static char command[COMMAND_DIM]; 
static void initializeShell();

static uint32_t color[] = { blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple,white};
static int colorIndex = 0;
static int actualColor = white;
static int actualBackgroundFont=black;

void shell (){
    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y-4*CHAR_HEIGHT);
    print("PROBANDO PRINTEAR EN PANTALLA DESDE USERLAND");
    initializeShell();
}

void initializeShell() {
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
    setFontColor(white);
    print("> ");
    setFontColor(actualColor);

    /*
    INTENTO DE MANEJO DE COMANDOS FALLIDO 
    VER COMO HACER PARA BORRAR (SYSCALL_BACKSPACE??)
    NO TIENE MUCHO SENTIDO QUE EL KERNEL TE DE DICHA SYSCALL IGUAL
    creo que es por este lado, pero tengo que ver porq me hace quilombo esta manera
    */
    //int index = 0;
    // char command[COMMAND_DIM];
    // while (!exit) {
    //     // Mostrar prompt
    //     setCursor(COMMAND_LINE_X, COMMAND_LINE_Y); 
    //     setFontColor(white);
    //     print("> ");
    //     setFontColor(actualColor);

    //     index = 0;

    //     while (1) {
    //         char c = getChar();  // asumimos que esta syscall ya está conectada

    //         if (c == '\n') {  // Enter
    //             command[index] = '\0';
    //             print("\n");
    //             // Ejecutar el comando
    //             runCommand(command);
    //             break;
    //         } else if (c == '\b') {  // Backspace
    //             if (index > 0) {
    //                 index--;
    //                 command[index] = ' ';
    //             }
    //         } else if (index < COMMAND_DIM - 1) {
    //             command[index++] = c;
    //         }
    //        // print(command);
    //     }
    // }
}


// no se si deberia estar aca, lo dudo
void cleanScreen() {
    drawRectangle((Point){1, COMMAND_LINE_Y-4*CHAR_HEIGHT}, (Point){DIM_X, DIM_Y}, 0x000000);
}

