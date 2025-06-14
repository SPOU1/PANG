#include "commonstuff.h"

#include "Obstacle.h"

Obstacle::Obstacle (float pos[3], float size[2]) {
    this->pos[X] = pos[X];
    this->pos[Y] = pos[Y];
    this->pos[Z] = pos[Z];

    rot[X] = 0;
    rot[Y] = 0;
    rot[Z] = 0;

    rspeed[X] = 0;
    rspeed[Y] = 0;
    rspeed[Z] = 0;

    tspeed[X] = 0;
    tspeed[Y] = 0;
    tspeed[Z] = 0;

    color[R] = 0.5f;
    color[G] = 0.5f;
    color[B] = 0.5f;

    acc_ratio = 0.0f;

    this->size[X] = size[X];
    this->size[Y] = size[Y];
}

void Obstacle::draw() {
    predraw();

    glScaled (size[X], size[Y], 1.0f);  // Escala el cubo según el tamaño especificado
    glutSolidCube (1);                  // Dibuja un cubo sólido con el tamaño especificado

    postdraw();
}

float Obstacle::getSize() {  // Área del obstáculo
    return size[X] * size[Y];
}

float Obstacle::getDistanceToEdge (float theta) {
    // Calcula la distancia al borde del obstáculo en la dirección theta
    float dist = 1 / sqrt (
                         pow (cos (theta), 2) / pow (size[X] / 2, 2) +
                         pow (sin (theta), 2) / pow (size[Y] / 2, 2));
    return dist;
}

float Obstacle::operator+ (Shape *other) {
    float otherPos[3];
    other->getPos (otherPos);
    float dist = mydistance (pos[X], pos[Y], otherPos[X], otherPos[Y]);
    return dist;
}