#include <commands.h>
#include <stdint.h>
#include <libc.h>

int strcmp(const char *s1, const char *s2);
void printString(const char *str);
int strncmp(const char *s1, const char *s2, size_t n);

// NO SE COMO FUNCIONA INCURSOR SI NO SE INCLUYE SHELL.H

int runCommand(const char *input)
{
    // setCursor(COMMAND_LINE_X, COMMAND_LINE_Y_exec);

    if (strcmp(input, "help") == 0)
    {
        print("Comandos disponibles:\n");
        print(" - help\n");
        print(" - clear\n");
        print(" - echo <mensaje>\n");
        print(" - date\n");
        print(" - zoom\n");
        print(" - div0 (causa una division por cero)\n");
        print(" - invop (causa una operacion invalida)\n");
        print(" - pongis golf\n");
        print(" - registers (muestra los registros del procesador)\n");
        return 1;
    }

    if (strcmp(input, "clear") == 0)
    {
        cleanScreen();
        return 2;
    }

    if (strncmp(input, "echo ", 5) == 0)
    {
        print(input + 5);
        putChar('\n');
        return 3;
    }

    if (strcmp(input, "date") == 0)
    {
        printDate();
        return 4;
    }

    if (strcmp(input, "div0") == 0)
    {
        _div();
    }

    if (strcmp(input, "invop") == 0)
    {
        _ioe();
    }

    // if (strcmp(input, "pongis golf") == 0)
    if (strcmp(input, "pg") == 0)
    {
        startPongis();
        return 5;
    }

    if (strcmp(input, "registers") == 0)
    {
        printRegisters();
        return 6;
    }

    if (strncmp(input, "zoom ", 5) == 0)
    {
        int new_zoom = atoi(input + 5);// atoi esta en el libc.c
            if (new_zoom < ZOOM_MIN || new_zoom > ZOOM_MAX)
            {
                print("New Zoom not valid!\n");
                return 7;
            }
            setZoom(new_zoom);
            cleanScreen();
            print("Zoom set to ");
            print(input + 5);
            print("\n");
        return 7;
    }

    print("Comando no reconocido\n");
    return -1; // Command not recognized
}
