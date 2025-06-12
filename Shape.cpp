#include "Shape.h"

Shape::Shape() {
    mynumber = RAND_DOM (0, 1000000);
}

void Shape::getPos (float *p) {
    p[X] = pos[X];
    p[Y] = pos[Y];
    p[Z] = pos[Z];
}

int Shape::getNo() {
    return mynumber;
}

void Shape::predraw() {
    glPushMatrix();
    glTranslatef (pos[X], pos[Y], pos[Z]);
    glRotatef (rot[X], 1, 0, 0);
    glRotatef (rot[Y], 0, 1, 0);
    glRotatef (rot[Z], 0, 0, 1);

    glColor3f (color[R], color[G], color[B]);
}

void Shape::postdraw() {
    glPopMatrix();
}

void Shape::move() {
    // Actualiza la posición
    pos[X] += tspeed[X];
    pos[Y] += tspeed[Y];
    pos[Z] += tspeed[Z];

    // Actualiza la rotación
    rot[X] += rspeed[X];
    rot[Y] += rspeed[Y];
    rot[Z] += rspeed[Z];

    // Aplica gravedad
    tspeed[Y] += GRAV * acc_ratio;

    // --- Colisión con el suelo ---
    if (pos[Y] <= -SIZEY2) {
        pos[Y] = -SIZEY2;

        // Rebota hacia arriba con toda la velocidad (sin amortiguación)
        tspeed[Y] = -tspeed[Y];
    }

    // --- Colisión con el techo ---
    if (pos[Y] >= SIZEY2) {
        pos[Y] = SIZEY2;
        tspeed[Y] = -tspeed[Y];
    }

    // --- Colisión con los lados ---
    if (pos[X] <= -SIZEX2) {
        pos[X] = -SIZEX2;
        tspeed[X] = -tspeed[X];
    }
    if (pos[X] >= SIZEX2) {
        pos[X] = SIZEX2;
        tspeed[X] = -tspeed[X];
    }

    // Limita las rotaciones a [0, 360]
    for (int i = 0; i < 3; i++) {
        if (rot[i] >= 360.0f) rot[i] -= 360.0f;
        if (rot[i] < 0.0f) rot[i] += 360.0f;
    }
}
