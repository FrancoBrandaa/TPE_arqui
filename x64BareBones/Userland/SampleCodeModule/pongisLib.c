#include <pongisLib.h>
#include <libc.h>

//todo lo que sean funciones especificas del juego pongis

    
void drawBall(Ball* b) {
    for (int i = -b->radius; i <= b->radius; i++) {
        for (int j = -b->radius; j <= b->radius; j++) {
            if (i*i + j*j <= b->radius * b->radius) {
                putPixel(b->color,b->x + i, b->y + j);
            }
        }
    }
}


//CHEQUEO DE COLISION ENTRE PELOTAS
int checkCollision(Ball* a, Ball* b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;

    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = a->radius + b->radius;

    return distanceSquared <= radiusSum * radiusSum;
}


void clearBall(Ball* b) {
    for (int i = -b->radius; i <= b->radius; i++) {
        for (int j = -b->radius; j <= b->radius; j++) {
            if (i*i + j*j <= b->radius * b->radius) {
                putPixel(0x000000, b->x + i, b->y + j); // negro o fondo
            }
        }
    }
}

void updateBall(Ball* b, int screenWidth, int screenHeight) {
    b->x += b->dx;
    b->y += b->dy;

    // Rebotar contra bordes
    if (b->x - b->radius <= 0 || b->x + b->radius >= screenWidth) //es un entero compardo con un int debemos poner una constante de comparacion
        b->dx *= -1;

    if (b->y - b->radius <= 0 || b->y + b->radius >= screenHeight)
        b->dy *= -1;
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





