#include <stdint.h>
#include <shell.h>
#include "./include/libc.h"
#include <libasm.h>
#include <commands.h>

static char exit = 0;
static void initializeShell();
   
void shell()
{
    initScreenSize(); 
    cleanScreen();
    setFontColor(white);
    setZoom(2);
    print("Los Monos TPE!\n");
    initializeShell();
}

void initializeShell()
{
    char command[COMMAND_DIM];
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


