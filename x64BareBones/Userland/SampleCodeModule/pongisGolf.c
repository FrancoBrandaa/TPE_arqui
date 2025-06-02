#include <libc.h>
#include <pongisLib.h>
#define ANGLES 360

// NO SE USA, NO LA BORRO POR PAJA
//  void setSpeed(Object * ball , float vx, float vy){
//          ball->dx = vx;
//          ball->dy = vy;
//  }

void startPongis()
{
    while (1)
    {
        cleanScreen();
        // franc se la come
        setZoom(4);
        setFontColor(blue);
        print("           Poro Golf\n\n");
        setZoom(3);
        print("    Presione Enter para empezar a jugar\n\n");
        print("          Presione Q para salir\n");
        printPoroMenu();
        char c = getChar();
        if (c != NOT_KEY)
        {
            if (c == '\n' || c == '\r')
            {
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
    srand_from_time(); // Inicializa la semilla aleatoria con la hora actual
    cleanScreen();

     Object ball = 
     {
        .x      = 100.0f,
        .y      = 100.0f,
        .dx     = 0.0f,
        .dy     = 0.0f,
        .speed  = 0.0f,      // La pelota comienza quieta
        .angle  = 0.0f,      // El ángulo no importa al inicio
        .radius = 20.0f,
        .color  = COLOR_GREEN
    };

    Object player = {
        .x      = 400.0f,
        .y      = 100.0f,
        .dx     = 0.0f,
        .dy     = 0.0f,
        .speed  = 0.0f,      // El jugador parte sin velocidad
        .angle  = 0.0f,      // Mirando hacia la derecha (0°)
        .radius = 20.0f,
        .color  = COLOR_WHITE
    };

    Object hole = {
    .x      = 100 + rand() % (1024 - 200), // entre 100 y 924
    .y      = 100 + rand() % (768 - 200),  // entre 100 y 668
    .dx     = 0.0f,                        // no hace falta dx/dy
    .dy     = 0.0f,
    .speed  = 0.0f,
    .angle  = 0.0f,
    .radius = 30.0f,
    .color  = COLOR_RED
};

    // Umbral al 60% para "caida en el hoyo"
    float threshold = hole.radius - 0.6f * ball.radius;
    float threshold2 = threshold * threshold;

    char lastRawKey = NOT_KEY;

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

        // 3) Leer rawKey y aplicar "debounce"
        char rawKey = getChar();
        char key;  // Tecla filtrada (sin repeticiones continuas)
        

        if (rawKey != NOT_KEY) {
            if (rawKey == lastRawKey) {
                // Es repetición automática: lo ignoramos
                key = NOT_KEY;
            } else {
                // Nueva pulsación distinta a la anterior
                key = rawKey;
                lastRawKey = rawKey;
            }
        } else {
            // No hay tecla en buffer: reiniciamos lastRawKey
            key = NOT_KEY;
            lastRawKey = NOT_KEY;
        }

        applyControls(&player, key);
        updateObject(&player, 1024, 768);

        if (key == KEY_ESC) {
        return;
        }

        if (key == NOT_KEY) {
            applyFriction(&player, 0.05f);
        }

        updateObject(&ball, 1024, 768);

        if (checkCollision(&player, &ball)) {
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
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i
                                     + (uint32_t)dy_i * (uint32_t)dy_i);

            //      4.3.3) Obtener dist = floor(sqrt(dist2)) con isqrt
            uint32_t dist = isqrt(dist2);
            if (dist == 0) {
                dist = 1;  // Para evitar división por cero
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
            // 3) Mostrar pantalla de victoria y salir del bucle
            cleanScreen();
            //setCursor(400, 350); // Ajusta posición del texto a tu gusto
            setZoom(3);
            setFontColor(red);
            print("\nGANASTE EL JUEGO!\n\n");
            print("Presiona cualquier tecla para salir...\n");
            while (getChar() == NOT_KEY); 
            startPongis(); // Reiniciar el juego
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
