#include <libc.h>
#include <pongisLib.h>
#define ANGLES 360
static int current_level = 1;
static const char* options[] = { "Jugar (1 jugador)", "Seleccionar Nivel" ,"Salir" };
static const int nopt = sizeof(options) / sizeof(options[0]);


static void drawMenu(int selected) 
{
    setBackGroundColor(black);
    cleanScreen();
    setZoom(5);
    setFontColor(blue);
    print("        Poro Golf\n\n");

    setZoom(3);
    for (int i = 0; i < nopt; i++) 
    {
        print(" ");
        print(options[i]);
        if (i == selected) print("  <--");
        print("\n\n");
    }

    printPoroMenu();
}

void startPongis() {
    int selected = 0;
    drawMenu(selected);

    while (1) {
        char c = getChar();
        if (c == NOT_KEY) continue;

        if (c == 'w') {               // subir
            selected = (selected + nopt - 1) % nopt;
            drawMenu(selected);
        }
        else if (c == 's') {          // bajar
            selected = (selected + 1) % nopt;
            drawMenu(selected);
        }
        else if (c == '\n' || c == '\r') {
            // 3) actúo según el índice seleccionado
            switch (selected) {
                case 0:  current_level = 1; startGame();    break;
                case 1:  shell();                           break;
                case 2:  shell();                           break;
                // case 3:  otraAcción();                       break;
                default: return;
            }
            return;
        }
    }
}

// void startPongis()
// {

//     cleanScreen();
//     // franc se la come
//     setZoom(4);
//     setFontColor(blue);
//     print("           Poro Golf\n\n");
//     setZoom(3);
//     print("    Presione Enter para empezar a jugar\n\n");
//     print("          Presione Q para salir\n");
//     printPoroMenu();

//     while (1)
//     {
//         char c = getChar();
//         if (c != NOT_KEY)
//         {
//             if (c == '\n' || c == '\r')
//             {
//                 current_level = 1;
//                 startGame();
//             }

//             if (c == 'q')
//             {
//                 shell();
//             }
//         }
//     }
// }

float radiusByLevel(int level)
{
    switch (level)
    {
    case 1:
        return 50.0f;
    case 2:
        return 40.0f;
    case 3:
        return 30.0f;
    default:
        return 30.0f;
    }
}

void win()
{
    setBackGroundColor(black);
    cleanScreen();
    setZoom(4);
    setFontColor(green);
    print("        NIVEL ");
    printInt(current_level);
    print(" SUPERADO\n\n");
    setZoom(3);
    (current_level < 3) ? print("Presione Enter para jugar el proximo\nnivel.\n") : print("Has completado el juego!\n");
    print("\nPresione Q para salir.\n");
    while (1)
    {
        char c = getChar();
        if (c != NOT_KEY)
        {
            if ((c == '\n' || c == '\r') && current_level < 3)
            {
                current_level++;
                startGame();
            }
            if (c == 'q')
            {
                shell();
            }
        }
    }
}


void startGame()
{
    setBackGroundColor(darkGreen);
    srand_from_time(); // Inicializa la semilla aleatoria con la hora actual
    cleanScreen();

    float hole_radius = radiusByLevel(current_level);

    Object ball =
        {
            .x = 100.0f,
            .y = 100.0f,
            .dx = 0.0f,
            .dy = 0.0f,
            .speed = 0.0f, // La pelota comienza quieta
            .angle = 0.0f, // El ángulo no importa al inicio
            .radius = 20.0f,
            .color = red};

    Object player = {
        .x = 400.0f,
        .y = 100.0f,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f, // El jugador parte sin velocidad
        .angle = 0.0f, // Mirando hacia la derecha (0°)
        .radius = 20.0f,
        .color = white};

    Object hole = {
        .x = 100 + rand() % (1024 - 200), // entre 100 y 924
        .y = 100 + rand() % (768 - 200),  // entre 100 y 668
        .dx = 0.0f,                       // no hace falta dx/dy
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = hole_radius, // Radio del hoyo según el nivel
        .color = black};

    // Umbral al 60% para "caida en el hoyo"
    float threshold = hole.radius - 0.6f * ball.radius;
    float threshold2 = threshold * threshold;

    while (1)
    {
        cleanScreen();

        for (int dy = -(int)hole.radius; dy <= (int)hole.radius; dy++)
        {
            for (int dx = -(int)hole.radius; dx <= (int)hole.radius; dx++)
            {
                if (dx * dx + dy * dy <= hole.radius * hole.radius)
                {
                    putPixel(hole.color, (uint64_t)(hole.x + dx), (uint64_t)(hole.y + dy));
                }
            }
        }

        applyControls(&player);
        updatePlayer(&player, 1024, 768); //magic numbers de los tamnaos cambiar

        if (!isKeyPressed('w'))
        {
            applyFriction(&player, 0.05f); //magic number
        }

        updateObject(&ball, 1024, 768); //magic number

        if (checkCollision(&player, &ball))
        {
            // 4.1) Copiar velocidad y dirección del player
            ball.speed = player.speed;
            ball.angle = player.angle;

            // 4.2) Recalcular dx/dy de la bola
            {
                float fx = (float)get_cos(ball.angle) / (1 << FIXED_SHIFT);
                float fy = (float)get_sin(ball.angle) / (1 << FIXED_SHIFT);
                ball.dx = ball.speed * fx;
                ball.dy = ball.speed * fy;
            }

            // 4.3) “Expulsar” la bola fuera del área de colisión sin usar floats:
            //      4.3.1) Convertir posiciones a enteros
            int bx = (int)ball.x;
            int by = (int)ball.y;
            int px = (int)player.x;
            int py = (int)player.y;

            //      4.3.2) Calcular dx_i, dy_i en enteros
            int dx_i = bx - px;
            int dy_i = by - py;
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i + (uint32_t)dy_i * (uint32_t)dy_i);

            //      4.3.3) Obtener dist = floor(sqrt(dist2)) con isqrt
            uint32_t dist = isqrt(dist2);
            if (dist == 0)
            {
                dist = 1; // Para evitar división por cero
            }

            //      4.3.4) Vector unitario en Q8.8 (factor = 256)
            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            //      4.3.5) Distancia a empujar = player.radius + ball.radius + 1
            int pushDist = (int)(player.radius + ball.radius + 1.0f);
            //               ^-- Si quieres redondear: + 0.5f antes de castear

            //      4.3.6) Calcular offset en Q8.8 y desplazar >> 8 para volver a entero
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            //      4.3.7) Recolocar la bola
            ball.x = (float)(px + offsetX);
            ball.y = (float)(py + offsetY);
        }

        updateObject(&ball, 1024, 768);

        applyFriction(&ball, 0.05f);

        float dx_h = ball.x - hole.x;
        float dy_h = ball.y - hole.y;
        float dist2 = dx_h * dx_h + dy_h * dy_h;

        if (dist2 <= threshold2)
        {
            win();
        }

        // applyFriction(&player, 0.5f);
        // si le aplico friccion a la pelota rompe (frena muy rapido??)
        // applyFriction(&ball, 0.05f);
        drawBall(&player);
        drawBall(&ball);
        sleep(10); // 20 ms → ~50 fps
    }
}

// START PONGIS PORO
//  void startPongis() {
//      const int scale       = 10;
//      const int spriteSz    = 32;
//      const int cx          = 100 + (spriteSz * scale)/2;
//      const int cy          = 100 + (spriteSz * scale)/2;
//      const int FRAME_DELAY = 1;  // 20 ms
//      const int STEP        = 5;      // grados por pulsación

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
