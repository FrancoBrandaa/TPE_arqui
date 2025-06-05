#include <pongisLib.h>
#include <libc.h>

// todo lo que sean funciones especificas del juego pongis


void drawBall(Object *b)
{
    // for (int i = -b->radius; i <= b->radius; i++)
    // {
    //     for (int j = -b->radius; j <= b->radius; j++)
    //     {
    //         if (i * i + j * j <= b->radius * b->radius)
    //         {
    //             putPixel(b->color, b->x + i, b->y + j);
    //         }
    //     }
    // }
    drawCircle(b->x, b->y, b->radius, b->color);
}

// float radiusByLevel(int level)
// {
//     switch (level)
//     {
//         case 1: return 50.0f;
//         case 2: return 40.0f;
//         case 3: return 30.0f;
//         default: return 30.0f;
//     }
// }

static unsigned int rand_seed = 2463534242U;

void srand_from_time() 
{
    dateStruct *date = getDate();
    // Combina hora, minuto y segundo para una semilla simple
    unsigned int seed = (date->hour << 16) | (date->min << 8) | date->sec;
    rand_seed = seed;
}

int rand() {
    rand_seed = rand_seed * 1664525U + 1013904223U;
    return (rand_seed >> 16) & 0x7FFF;
}

void srand(unsigned int seed) {
    rand_seed = seed;
}

void applyControls(Object *b, char keyPressed)
{
    float rotationStep = 25.0f; // Grados por tecla
    float acceleration = 2.0f;  // Aceleración por tecla
    float maxSpeed = 10.0f;     // Velocidad máxima

    if (keyPressed != NOT_KEY)
    {
        switch (keyPressed)
        {
        case 'w': // Acelera
            b->speed += acceleration;
            if (b->speed > maxSpeed)
                b->speed = maxSpeed;
            break;

        case 's': // Frena
            b->speed -= acceleration;
            if (b->speed < 0)
                b->speed = 0.0f;
            break;

        case 'a': // Rota a la izquierda
            b->angle -= rotationStep;
            break;

        case 'd': // Rota a la derecha
            b->angle += rotationStep;
            break;

        default:
            break;
        }

        float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
        float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
        b->dx = b->speed * fx;
        b->dy = b->speed * fy;
    }
}

// Antes tenías:
// void applyControls(Object *b, char keyPressed) { ... }

// Reemplázalo por esta nueva versión:

void applyControlsFlags(Object *b, bool wPressed, bool aPressed, bool sPressed, bool dPressed)
{
    float rotationStep = 25.0f;  // Grados por tecla
    float acceleration = 2.0f;   // Aceleración por frame si mantengo presionado
    float maxSpeed = 10.0f;      // Velocidad máxima

    // 1) Si 'w' está presionado, aumento la velocidad hasta maxSpeed:
    if (wPressed) {
        b->speed += acceleration;
        if (b->speed > maxSpeed)
            b->speed = maxSpeed;
    }

    // 2) Si 's' está presionado, disminuyo la velocidad hasta 0:
    if (sPressed) {
        b->speed -= acceleration;
        if (b->speed < 0.0f)
            b->speed = 0.0f;
    }

    // 3) Si 'a' está presionado, rotar a la izquierda:
    if (aPressed) {
        b->angle -= rotationStep;
    }

    // 4) Si 'd' está presionado, rotar a la derecha:
    if (dPressed) {
        b->angle += rotationStep;
    }

    // 5) Normalizar ángulo para que quede entre [0, 360):
    if (b->angle < 0.0f)       b->angle += 360.0f;
    if (b->angle >= 360.0f)   b->angle -= 360.0f;

    // 6) Recalcular componentes dx, dy según el nuevo ángulo y la velocidad:
    {
        float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
        float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
        b->dx = b->speed * fx;
        b->dy = b->speed * fy;
    }
}

// CHEQUEO DE COLISION ENTRE objetos (pelotas)
int checkCollision(Object *a, Object *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;

    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = a->radius + b->radius;

    return distanceSquared <= radiusSum * radiusSum;
}

// void clearBall(Object* b) {
//     for (int i = -b->radius; i <= b->radius; i++) {
//         for (int j = -b->radius; j <= b->radius; j++) {
//             if (i*i + j*j <= b->radius * b->radius) {
//                 putPixel(0x000000, b->x + i, b->y + j); // negro o fondo
//             }
//         }
//     }
// }
/**
 * Aplica una desaceleración continua (fricción) al objeto.
 * Si el objeto aún se mueve (speed diferente de 0), reduce su velocidad
 * en 'deceleration' unidades por frame, hasta llegar a cero.
 * Luego, recalcula dx y dy según el ángulo actual.
 *
 * Parámetros:
 *  - b: puntero al objeto al que se le aplica la fricción.
 *  - deceleration: magnitud de la desaceleración a aplicar cada frame.
 */
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

    // Recalcular componentes de velocidad en x e y según el ángulo
    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}

void updateObject(Object *b, int screenWidth, int screenHeight)
{
    // 1) Calculamos la posición que tendría la bola en el próximo frame
    float nextX = b->x + b->dx;
    float nextY = b->y + b->dy;
    float r = b->radius;
    float w = (float)screenWidth;
    float h = (float)screenHeight;

    // 2) Comprobamos rebote en el eje X
    if (nextX - r < 0.0f) {
        // Choque contra pared izquierda
        float penetration = r - nextX;
        // **Invertir dx**:
        b->dx = -b->dx;
        // **Calcular nuevo ángulo** (rebote horizontal)
        //  angle_new = 180° - angle_old
        b->angle = 180.0f - b->angle;
        if (b->angle < 0.0f) b->angle += 360.0f;
        if (b->angle >= 360.0f) b->angle -= 360.0f;
        // **Recalcular dx/dy a partir de angle y speed**:
        {
            float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
            float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
            b->dx = b->speed * fx;
            b->dy = b->speed * fy;
        }
        // Ajustar posición afuera del muro
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

void updatePlayer(Object *b, int screenWidth, int screenHeight) {
    float nextX = b->x + b->dx;
    float nextY = b->y + b->dy;
    float r     = b->radius;
    float w     = (float)screenWidth;
    float h     = (float)screenHeight;

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

void drawPoroRotated_LUT(int centerX, int centerY, int scale, int angleDeg)
{
    const int N = 32;
    const int half = N / 2;
    int s = get_sin(angleDeg);
    int c = get_cos(angleDeg);

    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            uint32_t color = poro_sprite[y * N + x];
            if (color == 0xFF00FF)
                continue;
            int dx = (x - half) * scale;
            int dy = (y - half) * scale;
            int rx = (dx * c - dy * s) >> FIXED_SHIFT;
            int ry = (dx * s + dy * c) >> FIXED_SHIFT;
            // rellena el bloque scale×scale para que no queden huecos
            for (int yy = 0; yy < scale; yy++)
                for (int xx = 0; xx < scale; xx++)
                    putPixel(color, centerX + rx + xx, centerY + ry + yy);
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

printPoroMenu(void)
{
    int scale = 10; // Cambia este valor para agrandar/reducir el sprite
    int sprite_size = 32;
    int offset_x = 650;
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

uint32_t isqrt(uint32_t x) {
    if (x == 0) {
        return 0;
    }

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
