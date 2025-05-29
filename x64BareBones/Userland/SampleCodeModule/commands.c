#include <commands.h>
#include <stdint.h>
#include <libc.h>


int strcmp(const char* s1, const char* s2);
void printString(const char* str);
int strncmp(const char* s1, const char* s2, size_t n);

//NO SE COMO FUNCIONA INCURSOR SI NO SE INCLUYE SHELL.H

int runCommand(const char *input) 
{
    //setCursor(COMMAND_LINE_X, COMMAND_LINE_Y_exec); 
    
    if (strcmp(input, "help") == 0) 
    {
        print("Comandos disponibles:\n");
        print(" - help\n");
        print(" - clear\n");
        print(" - echo <mensaje>\n");
        print(" - date\n");
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

    if (strncmp(input, "echo ", 5) == 0) {
        print(input + 5);
        putChar('\n');
        return 3;
    } 

    if (strcmp(input, "date") == 0)
    {
        printDate();
        return 4;
    }

    if (strcmp(input, "div0")==0) {
        _div();
    }
    
    if (strcmp(input, "invop")==0) {
        _ioe();
    }

    if (strcmp(input, "pongis golf") == 0) {
        print("Pongis Golf is a great game!\n");
        return 5;
    }

    if (strcmp(input, "registers") == 0) {
        printRegisters();
        //incCursorY(19-1); // Move cursor down after printing registers
        return 6;
    }
    
    print("Comando no reconocido\n");
    return -1; // Command not recognized
}



// oid doCommand() {
//     if (command[0] != '\0' && command[0] != '\n') {
//         commandDone = 1;
//         if (strCaseCmp(command, "color") == 0) {
//             actualColor=color[colorIndex];
//             setFontColor(actualColor);
//             colorIndex = (colorIndex+1)%21;
//             strCpy("New color setted", response);
//         } else if (strCaseCmp(command, "date")==0) {
//             cleanScreen();
//             programTime();
//             timeToStr(response);
//             getContextBack();
//         }else if (strCaseCmp(command, "rec")==0){
//             cleanScreen();
//             programRectangle(actualColor);
//             getContextBack();
//             strCpy("Rectangle exited", response);
//         } else if (strCaseCmp(command, "div0")==0) {
//             _div();
//         } else if (strCaseCmp(command, "invOp")==0) {
//             _ioe();
//         } else if (strCaseCmp(command, "help")==0) {
//             cleanScreen();
//             programHelp();
//             getContextBack();
//             strCpy("Help exited", response);
//         } else if (strCaseCmp(command, "zoom in") == 0) {
//             if (zoom_user < max_zoom) { 
//                 cleanScreen();
//                 strCpy("Zoomed in", response);
//                 setZoom(++zoom_user);
//                 getContextBack();
//             } else {
//                 strCpy("Max zoom possible", response);
//             }
//         } else if (strCaseCmp(command, "zoom out") == 0) {
//             if (zoom_user > min_zoom) { 
//                 cleanScreen();
//                 strCpy("Zoomed out", response);
//                 setZoom(--zoom_user);
//                 getContextBack();
//             } else {
//                 strCpy("Min zoom possible", response);
//             }
//         } else if (strCaseCmp(command, "snake")==0) {
//             cleanScreen();
//             snake();
//             getContextBack();
//             strCpy("Snake exited", response);
//         } else if (strCaseCmp(command, "registers")==0) {
//             cleanScreen();
//             programRegisters();
//             getContextBack();
//             strCpy("Registers exited", response);
//         } else if (strCaseCmp(command, "exit")==0) {
//             strCpy("Exit", response);
//             exit = 1;
//         } else {
//             strCpy("Command not found", response);
//         }
//     } else {
//         commandDone = 0;
//     }
// }