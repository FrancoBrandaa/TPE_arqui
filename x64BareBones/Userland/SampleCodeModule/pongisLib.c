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
    //drawCircle(b->x, b->y, b->radius, b->color);  
}

void drawPlayer(const Object *p, int scale) {
    // 1) Centramos la posición (convierte x,y float a int)
    int cx = (int)(p->x);
    int cy = (int)(p->y);

    // 2) Normalizamos el ángulo y opcionalmente aplicamos offset
    int angleDeg = (int)(p->angle) % 360;
    if (angleDeg < 0) angleDeg += 360;
    // Si el sprite original está “girado de costado”, descomenta:
        angleDeg = (angleDeg + 90) % 360;

    // 3) Dibujamos el Poro rotado y escalado
    drawPoroRotated_LUT(cx, cy, scale, angleDeg);
}

void drawPlayer2(const Object *p, int scale) {
    // 1) Centramos la posición (convierte x,y float a int)
    int cx = (int)(p->x);
    int cy = (int)(p->y);

    // 2) Normalizamos el ángulo y opcionalmente aplicamos offset
    int angleDeg = (int)(p->angle) % 360;
    if (angleDeg < 0) angleDeg += 360;
    // Si el sprite original está “girado de costado”, descomenta:
        angleDeg = (angleDeg + 90) % 360;

    // 3) Dibujamos el Poro rotado y escalado
    drawPoroRotated_LUT_2(cx, cy, scale, angleDeg);
}


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

void applyControls(Object *b)
{
    float rotationStep = 25.0f; // Grados por tecla
    float acceleration = 2.0f;  // Aceleración por tecla
    float maxSpeed = 10.0f;     // Velocidad máxima

    if (isKeyPressed('w')){
        b->speed += acceleration;
        if (b->speed > maxSpeed)
            b->speed = maxSpeed;
    }
    if(isKeyPressed('a')){
        b->angle -= rotationStep;
    }
    if (isKeyPressed('d')){
        b->angle += rotationStep;
    }

    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}

void applyControlsPlayer2(Object *b)
{
    float rotationStep = 25.0f;
    float acceleration = 2.0f;
    float maxSpeed = 10.0f;

    if (isKeyPressed('i'))
    {
        b->speed += acceleration;
        if (b->speed > maxSpeed)
            b->speed = maxSpeed;
    }
    
    if (isKeyPressed('j'))
    {
        b->angle -= rotationStep;
    }
    
    if (isKeyPressed('l'))
    {
        b->angle += rotationStep;
    }

    // if (b->angle < 0.0f)
    //     b->angle += 360.0f;
    // if (b->angle >= 360.0f)
    //     b->angle -= 360.0f;

    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}




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

    // Recalcular componentes de velocidad en x e y según el ángulo
    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}

void updateObject(Object *b)
{
    // 1) Calculamos la posición que tendría la bola en el próximo frame
    float nextX = b->x + b->dx;
    float nextY = b->y + b->dy;
    float r = b->radius;
    float w = (float)DIM_X;
    float h = (float)DIM_Y;

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

void drawPoroRotated_LUT(int centerX, int centerY, int scale, int angleDeg) {
    const int N    = 32;
    const int half = N / 2;

    // Normalizamos el ángulo a [0, 360)
    angleDeg %= 360;
    if (angleDeg < 0) angleDeg += 360;

    // Si el Poro en la matriz original está “de costado” por defecto,
    // podemos ajustar angleDeg con un offset. Por ejemplo, si el sprite
    // sin rotar aparece mirando hacia la derecha y quieres que 0° sea “arriba”,
    // usa angleDeg += 90; (o -= 90; según tu caso).
    //
    // Por ejemplo:
    //    angleDeg = (angleDeg + 90) % 360;
    //
    // Descomenta la línea anterior solo si efectivamente tu Poro original
    // está “girado 90°”.
    //
    // angleDeg = (angleDeg + 90) % 360;

    int s = get_sin(angleDeg); // Q15
    int c = get_cos(angleDeg); // Q15

    // Recorremos cada píxel de la matriz 32×32
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            uint32_t color = poro_sprite[y * N + x];
            if (color == 0xFF00FF) {
                // magenta = transparencia
                continue;
            }

            // 1) Coordenadas centradas en (0,0)
            int dx = x - half;  // rango: [-16 .. +15]
            int dy = y - half;  // rango: [-16 .. +15]

            // 2) Escalamos primero las coordenadas (para agrandar)
            int dx_s = dx * scale; 
            int dy_s = dy * scale;

            // 3) Rotamos la pareja (dx_s, dy_s) con ángulo angleDeg:
            //    [ rx ]   [  c  -s ] [ dx_s ]
            //    [ ry ] = [  s   c ] [ dy_s ]
            //
            //    Y como usamos Q15, desplazamos (>>) FIXED_SHIFT al final
            int rx = ((dx_s * c) - (dy_s * s)) >> FIXED_SHIFT;
            int ry = ((dx_s * s) + (dy_s * c)) >> FIXED_SHIFT;

            // 4) En pantalla, el píxel rotado y escalado cae en:
            //    (centerX + rx, centerY + ry). Si scale > 1, 
            //    cada punto rx,ry genera un bloque de scale×scale píxeles
            //    para que el sprite no se vea “disperso”.
            //
            //    Por simplicidad, dibujamos un bloque de tamaño `scale`
            //    con esquina en esa coordenada. Podrías centrar ese bloque
            //    restando scale/2, pero no es crítico para que el sprite se vea grande.
            //
            int baseX = centerX + rx;
            int baseY = centerY + ry;

            // Llenamos un bloque scale×scale (para “engrosar” el píxel)
            // Podrías dibujar solo 1×1, pero al rotar con escala a veces
            // quedan huecos; así queda compacto.
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

void drawPoroRotated_LUT_2(int centerX, int centerY, int scale, int angleDeg) {
    const int N    = 32;
    const int half = N / 2;

    // Normalizamos el ángulo a [0, 360)
    angleDeg %= 360;
    if (angleDeg < 0) angleDeg += 360;

    // Si el Poro en la matriz original está “de costado” por defecto,
    // podemos ajustar angleDeg con un offset. Por ejemplo, si el sprite
    // sin rotar aparece mirando hacia la derecha y quieres que 0° sea “arriba”,
    // usa angleDeg += 90; (o -= 90; según tu caso).
    //
    // Por ejemplo:
    //    angleDeg = (angleDeg + 90) % 360;
    //
    // Descomenta la línea anterior solo si efectivamente tu Poro original
    // está “girado 90°”.
    //
    // angleDeg = (angleDeg + 90) % 360;

    int s = get_sin(angleDeg); // Q15
    int c = get_cos(angleDeg); // Q15

    // Recorremos cada píxel de la matriz 32×32
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            uint32_t color = poro_sprite_2[y * N + x];
            if (color == 0xFF00FF) {
                // magenta = transparencia
                continue;
            }

            // 1) Coordenadas centradas en (0,0)
            int dx = x - half;  // rango: [-16 .. +15]
            int dy = y - half;  // rango: [-16 .. +15]

            // 2) Escalamos primero las coordenadas (para agrandar)
            int dx_s = dx * scale; 
            int dy_s = dy * scale;

            // 3) Rotamos la pareja (dx_s, dy_s) con ángulo angleDeg:
            //    [ rx ]   [  c  -s ] [ dx_s ]
            //    [ ry ] = [  s   c ] [ dy_s ]
            //
            //    Y como usamos Q15, desplazamos (>>) FIXED_SHIFT al final
            int rx = ((dx_s * c) - (dy_s * s)) >> FIXED_SHIFT;
            int ry = ((dx_s * s) + (dy_s * c)) >> FIXED_SHIFT;

            // 4) En pantalla, el píxel rotado y escalado cae en:
            //    (centerX + rx, centerY + ry). Si scale > 1, 
            //    cada punto rx,ry genera un bloque de scale×scale píxeles
            //    para que el sprite no se vea “disperso”.
            //
            //    Por simplicidad, dibujamos un bloque de tamaño `scale`
            //    con esquina en esa coordenada. Podrías centrar ese bloque
            //    restando scale/2, pero no es crítico para que el sprite se vea grande.
            //
            int baseX = centerX + rx;
            int baseY = centerY + ry;

            // Llenamos un bloque scale×scale (para “engrosar” el píxel)
            // Podrías dibujar solo 1×1, pero al rotar con escala a veces
            // quedan huecos; así queda compacto.
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
