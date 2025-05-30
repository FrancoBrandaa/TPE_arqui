#include <stdint.h>
#include <shell.h>
#include "./include/libc.h"
#include <libasm.h>
#include <commands.h>

#define COMMAND_DIM ((BASE_DIM_CHAR_X - 4) * 2) // maximo tamaño de comando, sacando margenes [EN CHARS BASE]
//#define COMMAND_DIM 256 // maximo tamaño de comando, sacando margenes [EN BYTES]


static char exit = 0;
static char command[COMMAND_DIM];
static void initializeShell();

static uint32_t color[] = {blue, green, red, yellow, purple, cyan, orange, pink, brown, 
    lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, 
    darkRed, darkYellow, darkPurple, white};

// void cmd_exit()
// {
//     exit = 1;
//     print("Exiting shell...\n");
// }
    
void shell()
{
    cleanScreen();
    //setCursor(0, 0); // Reset cursor position to the top-left corner
    setZoom(2);
    print("Los monos-1 TPE!\n");
    initializeShell();
}


void initializeShell()
{
    int index = 0;
    while (!exit)
    {
        print("  > ");
        index = 0;
        while (1)
        {
            char c = getChar();
            if (c != NOT_KEY)
            { 
                if (c == '\n' || c == '\r')
                { // Enter
                    command[index] = '\0';
                    print("\n");

                    // Execute the command
                    int num_comando = runCommand(command);
                    break;
                }
                else if (c == '\b' && index > 0) //se pasa del borrado 
                { // Backspace
                        // Elimina el último carácter del comando
                        putChar('\b'); // Mueve el cursor hacia atrás
                        index--;
                }
                else if (c != '\b' )// Comando no entra en la pantalla
                {
                    if(index < COMMAND_DIM - 1){
                        command[index++] = c;
                        putChar(c); // Muestra el carácter en la pantalla
                    }else{
                        print("Comando demasiado largo\n");
                        index = 0; // Reinicia el índice del comando
                        break;
                    }  
                }
                
            }
        }
    }
}

// while (true)
//         {
//             char c = getChar();
//             if (c == NOT_KEY)
//                 continue;

//             if (c == '\n' || c == '\r')
//             {
//                 // Fin de línea: cierra cadena y ejecuta comando
//                 command[index] = '\0';
//                 print("\n");
//                 int result = runCommand(command);
//                 (void)result;  // evitar warning si no se usa
//                 break;
//             }
//             else if (c == '\b')
//             {
//                 // Backspace: borra del buffer y de la pantalla
//                 if (index > 0)
//                 {
                        //ESTO LO HACE PABLITOOO
//                     index--;
//                     putChar('\b');
//                     putChar(' ');
//                     putChar('\b');
//                 }
//             }
//             else if (index < COMMAND_DIM - 1)
//             {
//                 // Carácter válido: almacena y muestra
//                 command[index++] = c;
//                 putChar(c);
//             }
//             else
//             {
//                 // Demasiado largo: informa, reinicia buffer
//                 print("\nComando demasiado largo\n");
//                 index = 0;
//                 break;
//             }


