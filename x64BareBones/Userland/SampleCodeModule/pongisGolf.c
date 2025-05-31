#include <libc.h>
#include <pongisLib.h>
#define ANGLES 360


//NO SE USA, NO LA BORRO POR PAJA
// void setSpeed(Object * ball , float vx, float vy){
//         ball->dx = vx;
//         ball->dy = vy;
// }

void startPongis()
{

    int scale = 6; // Cambia este valor para agrandar/reducir el sprite
    int sprite_size = 32;
    int offset_x = 50;
    int offset_y = 400;

    cleanScreen();
    // franc se la come
    setZoom(4);
    print("Poro Golf\n\n");
    setZoom(3);
    print("Presione enter para empezar a jugar\n\n");
    print("Presione q para Salir\n");


    for (int y = 0; y < sprite_size; y++)
    {
        for (int x = 0; x < sprite_size; x++)
        {
            uint32_t color = poro_sprite_32[y * sprite_size + x];
            if (color != 0xFF00FF) 
            {
                // Dibuja un bloque de scale x scale píxeles
                for (int dy = 0; dy < scale; dy++)
                {
                    for (int dx = 0; dx < scale; dx++)
                    {
                        putPixel(color, offset_x + x * scale + dx, offset_y + y * scale + dy);
                    }
                }
            }
        }
    }
    
    while (1)
    {
        char c = getChar();
        if (c != NOT_KEY)
        {
            if (c == '\n' || c == '\r')
            {
                startGame();
            }
            
            if(c == 'q')
            {
                shell();
            }
        }
        
    }

}


void startGame()
{
    cleanScreen();
    int angle = 0;       // en grados
    float speed = 3.0f;   // píxeles por frame

    Object ball = {
        .x      = 100.0f,
        .y      = 100.0f,
        .dx     = 0.0f,
        .dy     = 0.0f,
        .speed  = 0.0f,
        .angle  = .0f,
        .radius = 20.0f,
        .color  =  0x00FF00 //COLOR_GREEN
    };
    Object player = {
        .x      = 400.0f,
        .y      = 100.0f,
        .dx     = 0.0f,
        .dy     = 0.0f,
        .speed  = 0.0f,
        .angle  = 0.0f,
        .radius = 20.0f,
        .color  = 0xFFFFFF
    };
    while (1) {
        cleanScreen();
        applyControls(&player, getChar());
        updateObject(&player, 1024, 768);
        updateObject(&ball, 1024, 768);
        if (checkCollision(&player, &ball)) {
            ball.dx = player.dx;
            ball.dy = player.dy;
            player.dx -= 1 ;
            player.dy -= 1 ;
        }
       //applyFriction(&player, 0.5f);
       //si le aplico friccion a la pelota rompe (frena muy rapido??)
        //applyFriction(&ball, 0.05f);
        drawBall(&player);
        drawBall(&ball);
        sleep(10);   // 20 ms → ~50 fps
    }

    
    
}









//START PONGIS PORO
// void startPongis() {
//     const int scale       = 10;
//     const int spriteSz    = 32;
//     const int cx          = 100 + (spriteSz * scale)/2;
//     const int cy          = 100 + (spriteSz * scale)/2;
//     const int FRAME_DELAY = 1;  // 20 ms
//     const int STEP        = 5;      // grados por pulsación

//     int angle    = 0;
//     char lastKey = NOT_KEY;

//     // Dibujo inicial
//     cleanScreen();
//     drawPoroRotated_LUT(cx, cy, scale, angle);

//     while (1) {
//         char key = getChar();
//         // sólo si hay una tecla nueva y es distinta a la anterior
//         if (key != NOT_KEY && key != lastKey) {
//             if      (key == 'A' || key == 'a')
//             angle = (angle + ANGLES - STEP) % ANGLES;
//             else if (key == 'D' || key == 'd')
//             angle = (angle + STEP) % ANGLES;
//             else if (key == 'Q' || key == 'q')
//             break;
            
//             // redibujas el poro en la nueva orientación
//             cleanScreen();
//             drawPoroRotated_LUT(cx, cy, scale, angle);
//         }

//         lastKey = key;
//         // sleep(FRAME_DELAY);
//     }

//     cleanScreen();

// }
