#include <commands.h>

int runCommand(const char *input)
{
    // Limpiar el input eliminando espacios y caracteres ESC
    char cleanedInput[COMMAND_DIM];
    int i = 0, j = 0;
    
    while (input[i] != '\0') {
        // Skip spaces and ESC characters
        if (input[i] != ' ' && input[i] != 27) {
            cleanedInput[j] = input[i];
            j++;
        }
        i++;
    }
    cleanedInput[j] = '\0'; // Null terminate

    if (strcmp(cleanedInput, "help") == 0)
    {
        print("Comandos disponibles:\n");
        print(" - help\n");
        print(" - clear\n");
        print(" - echo <mensaje>\n");
        print(" - date\n");
        print(" - zoom <numero>\n");
        print(" - div0 (causa una division por cero)\n");
        print(" - invop (causa una operacion invalida)\n");
        print(" - pongis golf OR pg\n");
        print(" - color <color>\n");
        print(" - registers\n");
        return 1;
    }

    if (strcmp(cleanedInput, "clear") == 0)
    {
        cleanScreen();
        print("Los Monos TPE!\n");
        return 2;
    }

    // Para echo usar el input original para preservar espacios en el mensaje
    if (strncmp(input, "echo ", 5) == 0)
    {
        print(input + 5);
        putChar('\n');
        return 3;
    }

    if (strcmp(cleanedInput, "date") == 0)
    {
        printDate();
        return 4;
    }

    if (strcmp(cleanedInput, "div0") == 0)
    {
        _div();
        return 5;
    }

    if (strcmp(cleanedInput, "invop") == 0)
    {
        _ioe();
        return 6;
    }

    if (strcmp(cleanedInput, "pg") == 0 || strcmp(cleanedInput, "pongisgolf") == 0)
    {
        startPongis();
        return 7;
    }

    if (strcmp(cleanedInput, "registers") == 0)
    {
        printRegisters();
        return 8;
    }

    // Para comandos con argumentos, limpiar también
    if (strncmp(input, "zoom ", 5) == 0)
    {
        // Limpiar el argumento del zoom
        char cleanedZoom[COMMAND_DIM];
        int k = 5, l = 0; // Empezar después de "zoom "
        
        while (input[k] != '\0') {
            if (input[k] != ' ' && input[k] != 27) {
                cleanedZoom[l] = input[k];
                l++;
            }
            k++;
        }
        cleanedZoom[l] = '\0';
        
        int new_zoom = atoi(cleanedZoom);
        if (new_zoom < ZOOM_MIN || new_zoom > ZOOM_MAX)
        {
            print("New Zoom not valid!\n");
            return 9;
        }
        setZoom(new_zoom);
        cleanScreen();
        print("Zoom set to ");
        print(cleanedZoom);
        print("\n");
        return 9;
    }

    if (strncmp(input, "color ", 6) == 0)
    {
        // Limpiar el argumento del color
        char cleanedColor[COMMAND_DIM];
        int k = 6, l = 0; // Empezar después de "color "
        
        while (input[k] != '\0') {
            if (input[k] != ' ' && input[k] != 27) {
                cleanedColor[l] = input[k];
                l++;
            }
            k++;
        }
        cleanedColor[l] = '\0';
        
        uint32_t colorValue = white; 

        if (strcmp(cleanedColor, "black") == 0) colorValue = black;
        else if (strcmp(cleanedColor, "white") == 0) colorValue = white;
        else if (strcmp(cleanedColor, "blue") == 0) colorValue = blue;
        else if (strcmp(cleanedColor, "green") == 0) colorValue = green;
        else if (strcmp(cleanedColor, "red") == 0) colorValue = red;
        else {
            print("Color no reconocido\n");
            return 10;
        }

        setFontColor(colorValue);
        print("Color cambiado!\n");
        return 10;
    }

    print("Comando no reconocido\n");
    return -1;
}