#include<pongisLib.h>


//todo lo que sean funciones especificas del juego pongis

void drawBall(Ball* b) {
    for (int i = -b->radius; i <= b->radius; i++) {
        for (int j = -b->radius; j <= b->radius; j++) {
            if (i*i + j*j <= b->radius * b->radius) {
                putPixel(b->x + i, b->y + j, b->color);
            }
        }
    }
}

void clearBall(Ball* b) {
    for (int i = -b->radius; i <= b->radius; i++) {
        for (int j = -b->radius; j <= b->radius; j++) {
            if (i*i + j*j <= b->radius * b->radius) {
                putPixel(b->x + i, b->y + j, 0x000000); // negro o fondo
            }
        }
    }
}

void updateBall(Ball* b, int screenWidth, int screenHeight) {
    b->x += b->dx;
    b->y += b->dy;

    // Rebotar contra bordes
    if (b->x - b->radius <= 0 || b->x + b->radius >= screenWidth)
        b->dx *= -1;

    if (b->y - b->radius <= 0 || b->y + b->radius >= screenHeight)
        b->dy *= -1;
}