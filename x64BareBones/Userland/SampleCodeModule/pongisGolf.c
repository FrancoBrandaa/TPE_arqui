#include <libc.h>
#include <pongisLib.h>
#define ANGLES 360
static int current_level = 1;
static const char *options[] = {"Jugar (1 jugador)", "Jugar (2 jugadores)","Seleccionar Nivel", "Salir"};
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
        if (i == selected)
            print("  <--");
        print("\n\n");
    }

    printPoroMenu();
}

void startPongis()
{
    int selected = 0;
    drawMenu(selected);

    while (1)
    {
        char c = getChar();
        if (c == NOT_KEY)
            continue;

        if (c == 'w')
        { // subir
            selected = (selected + nopt - 1) % nopt;
            drawMenu(selected);
        }
        else if (c == 's')
        { // bajar
            selected = (selected + 1) % nopt;
            drawMenu(selected);
        }
        else if (c == '\n' || c == '\r')
        {
            // 3) actúo según el índice seleccionado
            switch (selected)
            {
            case 0:
                current_level = 1;
                startGame(1);
                break;
            case 1:
                startGame(2);
                break;
            case 2:
                shell();
                break;
            // case 3:  otraAcción();                       break;
            default:
                return;
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

void win(int numPlayers, int winner)
{
    setBackGroundColor(black);
    cleanScreen();
    setZoom(4);
    setFontColor(green);
    
    if (numPlayers == 2) {
        print("        JUGADOR ");
        printInt(winner);
        print(" GANA!\n");
        print("        NIVEL ");
        printInt(current_level);
        print(" SUPERADO\n\n");
    } else {
        print("        NIVEL ");
        printInt(current_level);
        print(" SUPERADO\n\n");
    }
    
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
                startGame(numPlayers);
            }
            if (c == 'q')
            {
                shell();
            }
        }
    }
}

void startGame(int numPlayers)
{
    setBackGroundColor(darkGreen);
    srand_from_time();
    cleanScreen();

    float hole_radius = radiusByLevel(current_level);

    Object ball = {
        .x = 100.0f,
        .y = 100.0f,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = 20.0f,
        .color = white};

    Object player1 = {
        .x = 400.0f,
        .y = 100.0f,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = 20.0f,
        .color = blue};

    Object player2 = {
        .x = 600.0f,
        .y = 200.0f,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = 20.0f,
        .color = red};

    Object hole = {
        .x = 100 + rand() % (1024 - 200),
        .y = 100 + rand() % (768 - 200),
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = hole_radius,
        .color = black};

    float threshold = hole.radius - 0.6f * ball.radius;
    float threshold2 = threshold * threshold;

    while (1)
    {
        cleanScreen();

        // Dibujar hoyo
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

        // Controles jugador 1
        applyControls(&player1);
        updatePlayer(&player1, 1024, 768);

        if (!isKeyPressed('w'))
        {
            applyFriction(&player1, 0.05f);
        }

        // Controles jugador 2 (solo si numPlayers == 2)
        if (numPlayers == 2)
        {
            applyControlsPlayer2(&player2);
            updatePlayer(&player2, 1024, 768);

            if (!isKeyPressed('i'))
            {
                applyFriction(&player2, 0.05f);
            }
        }

        updateObject(&ball, 1024, 768);

        // Colisión con jugador 1
        if (checkCollision(&player1, &ball))
        {
            ball.speed = player1.speed;
            ball.angle = player1.angle;

            float fx = (float)get_cos(ball.angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(ball.angle) / (1 << FIXED_SHIFT);
            ball.dx = ball.speed * fx;
            ball.dy = ball.speed * fy;

            // Separar la bola
            int bx = (int)ball.x;
            int by = (int)ball.y;
            int px = (int)player1.x;
            int py = (int)player1.y;

            int dx_i = bx - px;
            int dy_i = by - py;
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i + (uint32_t)dy_i * (uint32_t)dy_i);

            uint32_t dist = isqrt(dist2);
            if (dist == 0) dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(player1.radius + ball.radius + 1.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            ball.x = (float)(px + offsetX);
            ball.y = (float)(py + offsetY);
        }

        // Colisión con jugador 2 (solo si numPlayers == 2)
        if (numPlayers == 2 && checkCollision(&player2, &ball))
        {
            ball.speed = player2.speed;
            ball.angle = player2.angle;

            float fx = (float)get_cos(ball.angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(ball.angle) / (1 << FIXED_SHIFT);
            ball.dx = ball.speed * fx;
            ball.dy = ball.speed * fy;

            // Separar la bola
            int bx = (int)ball.x;
            int by = (int)ball.y;
            int px = (int)player2.x;
            int py = (int)player2.y;

            int dx_i = bx - px;
            int dy_i = by - py;
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i + (uint32_t)dy_i * (uint32_t)dy_i);

            uint32_t dist = isqrt(dist2);
            if (dist == 0) dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(player2.radius + ball.radius + 1.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            ball.x = (float)(px + offsetX);
            ball.y = (float)(py + offsetY);
        }

        applyFriction(&ball, 0.05f);

        float dx_h = ball.x - hole.x;
        float dy_h = ball.y - hole.y;
        float dist2 = dx_h * dx_h + dy_h * dy_h;


        if (dist2 <= threshold2)
        {
            playTone(523, 200);
            playTone(659, 200);
            playTone(784, 400);
            
            // Determinar quién hizo el último toque
            int winner = 1; // Por defecto jugador 1
            
            if (numPlayers == 2) {
                // Verificar quién está más cerca de la pelota para determinar el ganador
                float dist1 = (player1.x - ball.x) * (player1.x - ball.x) + (player1.y - ball.y) * (player1.y - ball.y);
                float dist2_p = (player2.x - ball.x) * (player2.x - ball.x) + (player2.y - ball.y) * (player2.y - ball.y);
                
                if (dist2_p < dist1)
                    winner = 2;
            }
            
            win(numPlayers, winner);
        }

        drawBall(&player1);
        if (numPlayers == 2)
        {
            drawBall(&player2);
        }
        drawBall(&ball);

        sleep(10);
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
