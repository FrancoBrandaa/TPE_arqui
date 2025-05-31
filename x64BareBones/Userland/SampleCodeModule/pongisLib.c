#include <pongisLib.h>

#include <libc.h>

//todo lo que sean funciones especificas del juego pongis

    
void drawBall(Object* b) {
    for (int i = -b->radius; i <= b->radius; i++) {
        for (int j = -b->radius; j <= b->radius; j++) {
            if (i*i + j*j <= b->radius * b->radius) {
                putPixel(b->color,b->x + i, b->y + j);
            }
        }
    }
}


void applyControls(Object* b, char keyPressed) {
    float rotationStep = 25.0f; // Grados por tecla
    float acceleration = 2.0f; // Aceleración por tecla
    float maxSpeed = 10.0f; // Velocidad máxima
    if(keyPressed != NOT_KEY){
    switch (keyPressed) {
        case 'w':  // Acelera
            b->speed += acceleration;
            if (b->speed > maxSpeed)
                b->speed = maxSpeed;
            break;

        case 's':  // Frena
            b->speed -= acceleration;
            if (b->speed < - maxSpeed )
                b->speed = - maxSpeed;
            break;

        case 'a':  // Rota a la izquierda
            b->angle -= rotationStep;
            break;

        case 'd':  // Rota a la derecha
            b->angle += rotationStep;
            break;

        default:
            break;
    }
    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT); 
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
    }else{
        applyFriction(b, 0.1f); // Aplicar fricción si no hay entrada
    }
}


//CHEQUEO DE COLISION ENTRE objetos (pelotas)
int checkCollision(Object* a, Object* b) {
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
void applyFriction(Object* b, float deceleration) {
    if (b->speed > 0.0f) {
        b->speed -= deceleration;
        if (b->speed < 0.0f) {
            b->speed = 0.0f;
        }
    } else if (b->speed < 0.0f) {
        b->speed += deceleration;
        if (b->speed > 0.0f) {
            b->speed = 0.0f;
        }
    }

    // Recalcular componentes de velocidad en x e y según el ángulo
    float fx = (float)get_cos(b->angle) / (1 << FIXED_SHIFT);
    float fy = (float)get_sin(b->angle) / (1 << FIXED_SHIFT);
    b->dx = b->speed * fx;
    b->dy = b->speed * fy;
}



void updateObject(Object* b, int screenWidth, int screenHeight)
{
    // 1) Calculamos la posición que tendría la bola en el próximo frame
    float nextX = b->x + b->dx;
    float nextY = b->y + b->dy;
    float r = b->radius;
    float w = (float)screenWidth;
    float h = (float)screenHeight;

    // 2) Comprobamos rebote en el eje X
    if (nextX - r < 0.0f) {
        // La bola habría atravesado el borde izquierdo
        float penetration = r - nextX;    // cuánto se “pasó” por la izquierda
        b->dx = -(b->dx);                    // invertimos la velocidad horizontal
        b->x  = r + penetration ;           // la colocamos justo fuera del borde
    }
    else if (nextX + r > w) {
        // La bola habría atravesado el borde derecho
        float penetration = nextX + r - w; // cuánto se “pasó” por la derecha
        b->dx = -(b->dx);                    // invertimos la velocidad horizontal
        b->x  = w - r - penetration ;       // la colocamos justo fuera del borde
    }
    else {
        // No colisiona horizontalmente: avanzamos normalmente
        b->x = nextX;
    }

    // 3) Comprobamos rebote en el eje Y
    if (nextY - r < 0.0f) {
        // La bola habría atravesado el borde superior
        float penetration = r - nextY;
        b->dy = -b->dy;                    // invertimos la velocidad vertical
        b->y  = r + penetration;           // la colocamos justo fuera del borde
    }
    else if (nextY + r > h) {
        // La bola habría atravesado el borde inferior
        float penetration = nextY + r - h;
        b->dy = -b->dy;                    // invertimos la velocidad vertical
        b->y  = h - r - penetration;       // la colocamos justo fuera del borde
    }
    else {
        // No colisiona verticalmente: avanzamos normalmente
        b->y = nextY;
    }
}



void drawPoroRotated_LUT(int centerX, int centerY, int scale, int angleDeg) 
{
  const int N    = 32;
  const int half = N/2;
  int s = get_sin(angleDeg);
  int c = get_cos(angleDeg);

  for (int y = 0; y < N; y++) {
    for (int x = 0; x < N; x++) {
      uint32_t color = poro_sprite[y * N + x];
      if (color == 0xFF00FF) continue;
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


int get_sin(int angle) {
    angle %= ANGLES;
    if (angle < 0) angle += ANGLES;
    int sector = angle / 90;
    int idx    = angle % 90;
    switch (sector) {
        case 0: return sin_table_q[idx];
        case 1: return sin_table_q[90 - idx];
        case 2: return -sin_table_q[idx];
        default: return -sin_table_q[90 - idx];
    }
}
 int get_cos(int angle) {return get_sin(angle + 90);}





