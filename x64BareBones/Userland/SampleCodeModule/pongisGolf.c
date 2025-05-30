#include <poroSprite.h>
#include <libc.h>
#include <poroSprite.h>    

#include <poroSprite.h>
#include <libc.h>

#include <poroSprite.h>
#include <libc.h>

void startPongis()
{
    cleanScreen();

    int scale = 10; // Cambia este valor para agrandar/reducir el sprite
    int sprite_size = 32;
    int offset_x = 100;
    int offset_y = 100;

    for (int y = 0; y < sprite_size; y++)
    {
        for (int x = 0; x < sprite_size; x++)
        {
            uint32_t color = poro_sprite_32[y * sprite_size + x];
            if (color != 0xFF00FF) {
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

    sleep(5);
    cleanScreen();
}


// void startPongis()
// {
//     int scale = 10;
//     int sprite_size = 32;
//     int offset_y = 100;
//     int frames = 20;
//     int step = 10;

//     int prev_offset_x = 100;

//     cleanScreen();

//     for (int frame = 0; frame < frames; frame++)
//     {
//         int offset_x = 100 + frame * step;

//         // Borra el sprite anterior (no toda la pantalla)
//         if (frame != 0) {
//             for (int y = 0; y < sprite_size * scale; y++)
//                 for (int x = 0; x < sprite_size * scale; x++)
//                     putPixel(0x000000, prev_offset_x + x, offset_y + y);
//         }

//         // Dibuja el sprite en la nueva posición
//         for (int y = 0; y < sprite_size; y++)
//         {
//             for (int x = 0; x < sprite_size; x++)
//             {
//                 uint32_t color = poro_sprite[y * sprite_size + x];
//                 if (color != 0xFF00FF) {
//                     for (int dy = 0; dy < scale; dy++)
//                         for (int dx = 0; dx < scale; dx++)
//                             putPixel(color, offset_x + x * scale + dx, offset_y + y * scale + dy);
//                 }
//             }
//         }
//         prev_offset_x = offset_x;
//         sleep(0.1);
//     }

//     sleep(1);
//     cleanScreen();
// }

// void startPongis()
// {
//     cleanScreen();

//     int scale = 10; // Cambia este valor para agrandar/reducir el sprite
//     int sprite_size = 32;
//     int offset_x = 100;
//     int offset_y = 100;

//     for (int y = 0; y < sprite_size; y++)
//     {
//         for (int x = 0; x < sprite_size; x++)
//         {
//             uint32_t color = poro_sprite_32[y * sprite_size + x];
//             if (color != 0xFF00FF) {
//                 // Dibuja un bloque de scale x scale píxeles
//                 for (int dy = 0; dy < scale; dy++)
//                 {
//                     for (int dx = 0; dx < scale; dx++)
//                     {
//                         putPixel(color, offset_x + x * scale + dx, offset_y + y * scale + dy);
//                     }
//                 }
//             }
//         }
//     }

//     sleep(5);
//     cleanScreen();
// }