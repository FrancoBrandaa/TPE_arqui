#include <pongisGolf.h>

// Configuración de niveles y dificultad
#define MAX_LEVELS 3
#define LEVEL_ARRAY_SIZE 4
#define DIFFICULTY_LEVELS 3
#define EASY_DIFFICULTY 0
#define NORMAL_DIFFICULTY 1
#define HARD_DIFFICULTY 2

// Posiciones iniciales de objetos
#define BALL_INITIAL_X 100.0f
#define BALL_INITIAL_Y 100.0f
#define PLAYER1_INITIAL_X 400.0f
#define PLAYER1_INITIAL_Y 100.0f
#define PLAYER2_INITIAL_X 600.0f
#define PLAYER2_INITIAL_Y 200.0f

// Radios de objetos
#define BALL_RADIUS 20.0f
#define PLAYER_RADIUS 20.0f
#define ROBOT_RADIUS 35.0f

// Configuración del robot orbital
#define ROBOT_ORBIT_OFFSET 80.0f
#define ROBOT_SPEED 2.0f

// Configuración de física
#define BALL_THRESHOLD_FACTOR 0.6f
#define PLAYER_FRICTION 0.3f
#define BALL_FRICTION 0.1f
#define ROBOT_PUSH_SPEED 8.0f
#define MIN_EXTRA_SPEED 5.0f
#define SEPARATION_OFFSET 5.0f
#define BALL_SEPARATION_OFFSET 1.0f

// Configuración de tiempo
#define FRAME_SLEEP_TIME 10

static int current_level = 1;
static int difficult = NORMAL_DIFFICULTY; // Dificultad inicial (0: Fácil, 1: Normal, 2: Difícil)
static int player1_collisions = 0;
static int player2_collisions = 0;

// Hay que tener criterio de cuando usamos snake o la otra  xd

static int MaxColitionsByLevel[4] =
    {
        // Maximo de colisiones por nivel
        0,  // Nivel 0 (no existe, solo para facilitar el acceso)
        15, // Nivel 1
        10, // Nivel 2
        5  // Nivel 3
};

static double radiusByDifficulty[3] =
    {
        // Radios por dificultad
        50.0, // Facil
        40.0, // Normal
        30.0  // Dificil
};

static const char *options[] =
    {
        "Jugar (1 jugador)",
        "Jugar (2 jugadores)",
        "dificultad",
        "Salir"};

static const int nopt = sizeof(options) / sizeof(options[0]);


// Función para obtener el nombre de la dificultad
static const char *getDifficultyName()
{
    switch (difficult)
    {
    case EASY_DIFFICULTY:
        return "Facil";
    case NORMAL_DIFFICULTY:
        return "Normal";
    case HARD_DIFFICULTY:
        return "Dificil";
    default:
        return "Normal";
    }
}

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
        if (i == 2)
        { // Opción de dificultad
            print("Dificultad: ");
            print(getDifficultyName());
        }
        else
        {
            print(options[i]);
        }

        if (i == selected)
            print("  <--");
        print("\n\n");
    }

    printPoroMenu();
    swapBuffers();
}

void startPongis()
{
    int selected = 0;
    drawMenu(selected);

    while (1)
    {
        char c = getChar();

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
                current_level = 1;
                startGame(2);
                break;
            case 2:
                // Cambiar dificultad cíclicamente
                difficult = (difficult + 1) % 3;
                drawMenu(selected);
                break;
            default:
                shell();
                return;
            }
            if (selected != 2) // Solo retornar si no es la opción de dificultad
                return;
        }
    }
}

void win(int numPlayers, int winner)
{
    setBackGroundColor(black);
    cleanScreen();
    setZoom(4);
    setFontColor(green);

    if (numPlayers == 2)
    {
        print("        JUGADOR ");
        printInt(winner);
        print(" GANA!\n");
        print("        NIVEL ");
        printInt(current_level);
        print(" SUPERADO\n\n");
    }
    else
    {
        print("        NIVEL ");
        printInt(current_level);
        print(" SUPERADO\n\n");
    }

    setZoom(3);
    (current_level < 3) ? print("    Enter para jugar el siguiente nivel\n") : print("          Has completado el juego!\n");
    print("\n              Q para salir\n");

    swapBuffers();

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

// Función para mostrar Game Over
void lose()
{
    setBackGroundColor(black);
    cleanScreen();
    setZoom(5);
    setFontColor(red);
    print("        GAME OVER\n\n");

    // Música de derrota
    playTone(659, 300); // E5
    playTone(622, 300); // D#5
    playTone(587, 300); // D5
    playTone(554, 300); // C#5
    playTone(523, 400); // C5
    playTone(493, 400); // B4
    playTone(440, 600); // A4

    setZoom(3);
    print("     Presione Q para volver al menu.\n");

    swapBuffers();

    while (1)
    {
        char c = getChar();
        if (c != NOT_KEY && c == 'q')
        {
            player1_collisions = 0;
            player2_collisions = 0;
            startPongis();
            return;
        }
    }
}

// Función para mostrar el contador de colisiones
void drawCollisionCounter(int numPlayers)
{
    setZoom(2);
    setFontColor(white);

    // Posición en la esquina superior izquierda
    setCursor(0, 0);

    print("J1: ");
    printInt(player1_collisions);
    print("/");
    printInt(MaxColitionsByLevel[current_level]);

    if (numPlayers == 2)
    {
        print("  J2: ");
        printInt(player2_collisions);
        print("/");
        printInt(MaxColitionsByLevel[current_level]);
    }
}

void startGame(int numPlayers)
{
    setBackGroundColor(darkGreen);
    srand_from_time();
    cleanScreen();

    double x_hole = 100 + rand() % (DIM_X - 200);
    double y_hole = 100 + rand() % (DIM_Y - 200);

    // Resetear contadores de colisiones al inicio del juego
    player1_collisions = 0;
    player2_collisions = 0;

Object ball = {
        .x = BALL_INITIAL_X,
        .y = BALL_INITIAL_Y,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = BALL_RADIUS,
        .color = white};

    Object player1 = {
        .x = PLAYER1_INITIAL_X,
        .y = PLAYER1_INITIAL_Y,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = PLAYER_RADIUS,
        .color = blue};

    Object player2 = {
        .x = PLAYER2_INITIAL_X,
        .y = PLAYER2_INITIAL_Y,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = PLAYER_RADIUS,
        .color = red};

    Object hole = {
        .x = x_hole,
        .y = y_hole,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = 0.0f,
        .angle = 0.0f,
        .radius = radiusByDifficulty[difficult],
        .color = black};

    // Robot que orbita alrededor del hoyo
    float orbitRadius = hole.radius + ROBOT_ORBIT_OFFSET; // Radio de órbita alrededor del hoyo
    Object robot = {
        .x = x_hole + orbitRadius, // Posición inicial en la órbita
        .y = y_hole,
        .dx = 0.0f,
        .dy = 0.0f,
        .speed = ROBOT_SPEED, // Velocidad angular del robot
        .angle = 0.0f, // Ángulo orbital inicial
        .radius = ROBOT_RADIUS,
        .color = darkGrey};

    float threshold = hole.radius - BALL_THRESHOLD_FACTOR * ball.radius;
    float threshold2 = threshold * threshold;

    float robotOrbitAngle = 0.0f; // Ángulo para la órbita del robot


    while (1)
    {
        // ESC para volver al menú
        if (getChar() == KEY_ESC)
        {
            player1_collisions = 0;
            player2_collisions = 0;
            // Limpio buffer
            while (getChar() != NOT_KEY);
            startPongis();
        }

        cleanScreen();

        // Verificar si algun jugador se quedo sin intentos
        if (numPlayers == 1)
        {
            if (player1_collisions >= MaxColitionsByLevel[current_level])
            {
                lose();
            }
        }
        else
        { // numPlayers == 2
            if (player1_collisions >= MaxColitionsByLevel[current_level] && player2_collisions >= MaxColitionsByLevel[current_level])
            {
                lose();
            }
        }

        // Actualizar movimiento orbital del robot
        robotOrbitAngle += robot.speed; // Incrementar ángulo orbital
        if (robotOrbitAngle >= 360.0f)
            robotOrbitAngle -= 360.0f;

        // Calcular nueva posición del robot en órbita circular
        float fx_orbit = (float)get_cos((int)robotOrbitAngle) / (1 << FIXED_SHIFT);
        float fy_orbit = (float)get_sin((int)robotOrbitAngle) / (1 << FIXED_SHIFT);
        
        robot.x = x_hole + orbitRadius * fx_orbit;
        robot.y = y_hole + orbitRadius * fy_orbit;

        updateObject(&robot);

        // Dibujar hoyo (croto mal)
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

        // Controles jugador 1 (solo si no se quedo sin intentos)
        if (player1_collisions < MaxColitionsByLevel[current_level])
        {
            applyControls(&player1, 1);
            updatePlayer(&player1);

            if (!isKeyPressed('w'))
            {
                applyFriction(&player1, 0.3f);
            }
        }

        // Controles jugador 2 (solo si numPlayers == 2 y no se quedo sin intentos)
        if (numPlayers == 2 && player2_collisions < MaxColitionsByLevel[current_level])
        {
            applyControls(&player2, 2);
            updatePlayer(&player2);

            if (!isKeyPressed('i'))
            {
                applyFriction(&player2, 0.3f);
            }
        }

        updateObject(&ball);

        // Colisión robot con pelota
        if (checkCollision(&robot, &ball))
        {
            // El robot empuja la pelota en dirección aleatoria
            int pushAngle = rand() % 360;
            ball.speed = 8.0f; // Velocidad del empujón
            ball.angle = pushAngle;

            float fx = (float)get_cos(ball.angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(ball.angle) / (1 << FIXED_SHIFT);
            ball.dx = ball.speed * fx;
            ball.dy = ball.speed * fy;

            // Separar la bola del robot
            int bx = (int)ball.x;
            int by = (int)ball.y;
            int rx = (int)robot.x;
            int ry = (int)robot.y;

            int dx_i = bx - rx;
            int dy_i = by - ry;
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i + (uint32_t)dy_i * (uint32_t)dy_i);

            uint32_t dist = isqrt(dist2);
            if (dist == 0)
                dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(robot.radius + ball.radius + 5.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            ball.x = (float)(rx + offsetX);
            ball.y = (float)(ry + offsetY);
        }

        // Colision robot con jugador 1
        if (checkCollision(&robot, &player1))
        {
            // El robot empuja al jugador
            int bx = (int)player1.x;
            int by = (int)player1.y;
            int rx = (int)robot.x;
            int ry = (int)robot.y;

            int dx_i = bx - rx;
            int dy_i = by - ry;
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i + (uint32_t)dy_i * (uint32_t)dy_i);

            uint32_t dist = isqrt(dist2);
            if (dist == 0)
                dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(robot.radius + player1.radius + 5.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            player1.x = (float)(rx + offsetX);
            player1.y = (float)(ry + offsetY);
        }

        // Colision robot con jugador 2 (si hay 2 jugadores)
        if (numPlayers == 2 && checkCollision(&robot, &player2))
        {
            // El robot empuja al jugador 2
            int bx = (int)player2.x;
            int by = (int)player2.y;
            int rx = (int)robot.x;
            int ry = (int)robot.y;

            int dx_i = bx - rx;
            int dy_i = by - ry;
            uint32_t dist2 = (uint32_t)((uint32_t)dx_i * (uint32_t)dx_i + (uint32_t)dy_i * (uint32_t)dy_i);

            uint32_t dist = isqrt(dist2);
            if (dist == 0)
                dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(robot.radius + player2.radius + 5.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            player2.x = (float)(rx + offsetX);
            player2.y = (float)(ry + offsetY);
        }

        // Colision con jugador 1 (solo si no se quedo sin intentos)
        if (player1_collisions < MaxColitionsByLevel[current_level] && checkCollision(&player1, &ball))
        {
            player1_collisions++; // Incrementar contador de colisiones

            float minExtra = 5.0f;
            ball.speed = player1.speed + minExtra;
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
            if (dist == 0)
                dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(player1.radius + ball.radius + 1.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            ball.x = (float)(px + offsetX);
            ball.y = (float)(py + offsetY);
        }

        // Colision con jugador 2 (solo si numPlayers == 2 y no se quedo sin intentos)
        if (numPlayers == 2 && player2_collisions < MaxColitionsByLevel[current_level] && checkCollision(&player2, &ball))
        {
            player2_collisions++; // Incrementar contador de colisiones

            float minExtra = 5.0f;
            ball.speed = player2.speed + minExtra;
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
            if (dist == 0)
                dist = 1;

            int ux_q8 = (dx_i * 256) / (int)dist;
            int uy_q8 = (dy_i * 256) / (int)dist;

            int pushDist = (int)(player2.radius + ball.radius + 1.0f);
            int offsetX = (ux_q8 * pushDist) >> 8;
            int offsetY = (uy_q8 * pushDist) >> 8;

            ball.x = (float)(px + offsetX);
            ball.y = (float)(py + offsetY);
        }

        applyFriction(&ball, 0.1f);

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

            if (numPlayers == 2)
            {
                // Verificar quién está más cerca de la pelota para determinar el ganador
                float dist1 = (player1.x - ball.x) * (player1.x - ball.x) + (player1.y - ball.y) * (player1.y - ball.y);
                float dist2_p = (player2.x - ball.x) * (player2.x - ball.x) + (player2.y - ball.y) * (player2.y - ball.y);

                if (dist2_p < dist1)
                    winner = 2;
            }

            // Resetear contadores antes de pasar al siguiente nivel
            player1_collisions = 0;
            player2_collisions = 0;

            win(numPlayers, winner);
        }

        // Dibujar todos los objetos
        drawBall(&robot); // Dibujar el robot

        drawPlayer(&player1, 2, 1);
        if (numPlayers == 2)
            drawPlayer(&player2, 2, 2);
        drawBall(&ball);

        // Mostrar contador de colisiones
        drawCollisionCounter(numPlayers);

        swapBuffers();
        sleep(FRAME_SLEEP_TIME);
    }
}