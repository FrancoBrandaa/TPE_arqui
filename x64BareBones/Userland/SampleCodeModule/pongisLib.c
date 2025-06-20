#include <pongisLib.h>
#include <libc.h>

// todo lo que sean funciones especificas del juego pongis
static unsigned int rand_seed = 2463534242U;

void drawBall(Object *b)
{
    for (int i = -b->radius; i <= b->radius; i++)
    {
        for (int j = -b->radius; j <= b->radius; j++)
        {
            if (i * i + j * j <= b->radius * b->radius)
            {
                putPixel(b->color, b->x + i, b->y + j);
            }
        }
    }
}

void drawPlayer(const Object *p, int scale, int player) 
{
    int cx = (int)(p->x);
    int cy = (int)(p->y);

    int angleDeg = (int)(p->angle) % 360;

    if (angleDeg < 0) angleDeg += 360;
        angleDeg = (angleDeg + 90) % 360;

    drawPoroRotated(cx, cy, scale, angleDeg, player);
}

void srand_from_time() 
{
    dateStruct *date = getDate();
    // Combina hora, minuto y segundo para una semilla simple
    unsigned int seed = (date->hour << 16) | (date->min << 8) | date->sec;
    rand_seed = seed;
}

int rand() 
{
    rand_seed = rand_seed * 1664525U + 1013904223U;
    return (rand_seed >> 16) & 0x7FFF;
}

void srand(unsigned int seed) {
    rand_seed = seed;
}

void applyControls(Object *b, int player)
{
    float rotationStep = 25.0f; // Grados por tecla
    float acceleration = 2.0f;  // Aceleración por tecla
    float maxSpeed = 10.0f;     // Velocidad máxima

    if ((player == 1 && isKeyPressed('w')) || (player == 2 && isKeyPressed('i'))){
        b->speed += acceleration;
        if (b->speed > maxSpeed)
            b->speed = maxSpeed;
    }
    if ((player == 1 && isKeyPressed('a')) || (player == 2 && isKeyPressed('j'))){
        b->angle -= rotationStep;
    }
    if ((player == 1 && isKeyPressed('d')) || (player == 2 && isKeyPressed('l'))){
        b->angle += rotationStep;
    }

    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}


int checkCollision(Object *a, Object *b)
{
    
    float dx = a->x - b->x;
    float dy = a->y - b->y;

    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = a->radius + b->radius;

    return distanceSquared <= radiusSum * radiusSum;
}


void applyFriction(Object *b, float deceleration)
{
    if (b->speed > 0.0f)
    {
        b->speed -= deceleration;
        if (b->speed < 0.0f)
        {
            b->speed = 0.0f;
        }
    }
    else if (b->speed < 0.0f)
    {
        b->speed += deceleration;
        if (b->speed > 0.0f)
        {
            b->speed = 0.0f;
        }
    }

    // Recalcular componentes de velocidad en x e y segun el angulo
    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}

void updateObject(Object *b)
{
    // Calculamos la posicion que tendria la bola en el proximo frame
    float nextX = b->x + b->dx;
    float nextY = b->y + b->dy;
    float r = b->radius;
    float w = (float)DIM_X;
    float h = (float)DIM_Y;

    // Comprobamos rebote en el eje X
    if (nextX - r < 0.0f) {
        // Choque contra pared izquierda
        float penetration = r - nextX;
        
        b->dx = -b->dx;
        
        b->angle = 180.0f - b->angle;
        if (b->angle < 0.0f) b->angle += 360.0f;
        if (b->angle >= 360.0f) b->angle -= 360.0f;
        {
            float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
            b->dx = b->speed * fx;
            b->dy = b->speed * fy;
        }
    
        b->x = r + penetration;
    }
    else if (nextX + r > w) {
        // Choque contra pared derecha
        float penetration = nextX + r - w;
        b->dx = -b->dx;
        // angle_new = 180° - angle_old
        b->angle = 180.0f - b->angle;
        if (b->angle < 0.0f) b->angle += 360.0f;
        if (b->angle >= 360.0f) b->angle -= 360.0f;
        // Recalcular dx/dy
        {
            float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
            b->dx = b->speed * fx;
            b->dy = b->speed * fy;
        }
        b->x = w - r - penetration;
    }
    else {
        // Avanzar normalmente en X
        b->x = nextX;
    }

    // 3) Comprobamos rebote en el eje Y
    if (nextY - r < 0.0f) {
        // Choque contra pared superior
        float penetration = r - nextY;
        b->dy = -b->dy;
        // angle_new = -angle_old  (reflexión vertical)
        b->angle = -b->angle;
        if (b->angle < 0.0f) b->angle += 360.0f;
        if (b->angle >= 360.0f) b->angle -= 360.0f;
        {
            float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
            b->dx = b->speed * fx;
            b->dy = b->speed * fy;
        }
        b->y = r + penetration;
    }
   else if (nextY + r > h) {
        // Choque contra pared inferior
        float penetration = nextY + r - h;
        b->dy = -b->dy;
        // angle_new = -angle_old
        b->angle = -b->angle;
        if (b->angle < 0.0f) b->angle += 360.0f;
        if (b->angle >= 360.0f) b->angle -= 360.0f;
        {
            float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
            b->dx = b->speed * fx;
            b->dy = b->speed * fy;
        }
        b->y = h - r - penetration;
    }
    else {
        // Avanzar normalmente en Y
        b->y = nextY;
    }
}

void updatePlayer(Object *b) {
    float nextX = b->x + b->dx;
    float nextY = b->y + b->dy;
    float r     = b->radius;
    float w     = (float)DIM_X;
    float h     = (float)DIM_Y;

    // ———————————————————————
    // 1) EJE X: colisión vs paredes izquierda/derecha
    // ———————————————————————
    if (nextX - r < 0.0f) {
        // Choque contra pared izquierda
        b->x  = r;        // “Clampeamos” la posición a la coordenada mínima
        b->dx = 0.0f;     // Anulamos solo la componente horizontal
        // b->dy queda igual → si hay componente vertical, desliza
    }
    else if (nextX + r > w) {
        // Choque contra pared derecha
        b->x  = w - r;    // Clampeamos a la coordenada máxima
        b->dx = 0.0f;     // Anulamos solo la componente horizontal
    }
    else {
        // No hay colisión: avanzar normalmente en X
        b->x = nextX;
    }

    // ———————————————————————
    // 2) EJE Y: colisión vs paredes superior/inferior
    // ———————————————————————
    if (nextY - r < 0.0f) {
        // Choque contra pared superior
        b->y  = r;        // Clampeamos
        b->dy = 0.0f;     // Anulamos solo la componente vertical
        // b->dx queda igual → si hay componente horizontal, desliza
    }
    else if (nextY + r > h) {
        // Choque contra pared inferior
        b->y  = h - r;    // Clampeamos
        b->dy = 0.0f;     // Anulamos solo la componente vertical
    }
    else {
        // No hay colisión: avanzar normalmente en Y
        b->y = nextY;
    }
}

void drawPoroRotated(int centerX, int centerY, int scale, int angleDeg, int player) 
{
    const int N    = 32;
    const int half = N / 2;

    // Normalizamos el ángulo a [0, 360)
    angleDeg %= 360;
    if (angleDeg < 0) angleDeg += 360;


    int s = get_sin(angleDeg); // Q15
    int c = get_cos(angleDeg); // Q15

    // Recorremos cada píxel de la matriz 32×32
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) 
        {
            // Seleccionamos el sprite según el jugador
            uint32_t color;
            if (player == 1)
                 color = poro_sprite[y * N + x];
            else
                 color = poro_sprite_2[y * N + x];

            if (color == 0xFF00FF) {
                // magenta = transparencia
                continue;
            }
            // Coordenadas centradas en (0,0)
            int dx = x - half;  // rango: [-16 .. +15]
            int dy = y - half;  // rango: [-16 .. +15]

            int dx_s = dx * scale; 
            int dy_s = dy * scale;

            int rx = ((dx_s * c) - (dy_s * s)) >> FIXED_SHIFT;
            int ry = ((dx_s * s) + (dy_s * c)) >> FIXED_SHIFT;

            int baseX = centerX + rx;
            int baseY = centerY + ry;

            for (int yy = 0; yy < scale; yy++) {
                for (int xx = 0; xx < scale; xx++) {
                    putPixel(color,
                             (uint64_t)(baseX + xx),
                             (uint64_t)(baseY + yy));
                }
            }
        }
    }
}

int get_sin(int angle)
{
    angle %= ANGLES;
    if (angle < 0)
        angle += ANGLES;
    int sector = angle / 90;
    int idx = angle % 90;
    switch (sector)
    {
    case 0:
        return sin_table_q[idx];
    case 1:
        return sin_table_q[90 - idx];
    case 2:
        return -sin_table_q[idx];
    default:
        return -sin_table_q[90 - idx];
    }
}
int get_cos(int angle) { return get_sin(angle + 90); }

void printPoroMenu(void)
{
    int scale = 10; // Cambia este valor para agrandar/reducir el sprite
    int sprite_size = 32;
    int offset_x = 600;
    int offset_y = 100;

    for (int y = 0; y < sprite_size; y++)
    {
        for (int x = 0; x < sprite_size; x++)
        {
            uint32_t color = poro_menu[y * sprite_size + x];
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
}

uint32_t isqrt(uint32_t x) 
{
    if (x == 0)
        return 0;

    uint32_t op  = x;
    uint32_t res = 0;
    uint32_t one = 1u << 30;  // Mayor potencia de 4 ≤ 2^31

    while (one > op) {
        one >>= 2;
    }

    while (one) {
        if (op >= res + one) {
            op  = op - (res + one);
            res = (res >> 1) + one;
        } else {
            res = (res >> 1);
        }
        one >>= 2;
    }

    return res;
}
