#include <commands.h>


int runCommand(const char *input)
{
    

    if (strcmp(input, "help") == 0)
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

    if (strcmp(input, "clear") == 0)
    {
        cleanScreen();
        print("Los Monos TPE!\n");
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
        return 5;
    }

    if (strcmp(input, "invop") == 0)
    {
        _ioe();
        return 6;
    }

    if (strcmp(input, "pg") == 0 || strcmp(input, "pongis golf") == 0)
    {
        startPongis();
        return 7;
    }

    if (strcmp(input, "registers") == 0)
    {
        printRegisters();
        return 8;
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
        return 9;
    }

    
    if (strncmp(input, "color ", 6) == 0)
    {
        const char *colorName = input + 6;
        uint32_t colorValue = white; 

        if (strcmp(colorName, "black") == 0) colorValue = black;
        else if (strcmp(colorName, "white") == 0) colorValue = white;
        else if (strcmp(colorName, "blue") == 0) colorValue = blue;
        else if (strcmp(colorName, "green") == 0) colorValue = green;
        else if (strcmp(colorName, "red") == 0) colorValue = red;
        else if (strcmp(colorName, "yellow") == 0) colorValue = yellow;
        else if (strcmp(colorName, "purple") == 0) colorValue = purple;
        else if (strcmp(colorName, "cyan") == 0) colorValue = cyan;
        else if (strcmp(colorName, "orange") == 0) colorValue = orange;
        else if (strcmp(colorName, "pink") == 0) colorValue = pink;
        else if (strcmp(colorName, "brown") == 0) colorValue = brown;
        else if (strcmp(colorName, "lightGrey") == 0) colorValue = lightGrey;
        else if (strcmp(colorName, "lightBlue") == 0) colorValue = lightBlue;
        else if (strcmp(colorName, "lightGreen") == 0) colorValue = lightGreen;
        else if (strcmp(colorName, "lightRed") == 0) colorValue = lightRed;
        else if (strcmp(colorName, "lightPink") == 0) colorValue = lightPink;
        else if (strcmp(colorName, "lightBrown") == 0) colorValue = lightBrown;
        else if (strcmp(colorName, "darkBlue") == 0) colorValue = darkBlue;
        else if (strcmp(colorName, "darkGreen") == 0) colorValue = darkGreen;
        else if (strcmp(colorName, "darkRed") == 0) colorValue = darkRed;
        else if (strcmp(colorName, "darkYellow") == 0) colorValue = darkYellow;
        else if (strcmp(colorName, "darkPurple") == 0) colorValue = darkPurple;
        else if (strcmp(colorName, "darkCyan") == 0) colorValue = darkCyan;
        else if (strcmp(colorName, "darkOrange") == 0) colorValue = darkOrange;
        else if (strcmp(colorName, "darkPink") == 0) colorValue = darkPink;
        else if (strcmp(colorName, "darkBrown") == 0) colorValue = darkBrown;
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
