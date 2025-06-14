#include "commonstuff.h"
#include "Ball.h"
#include "ObjectsList.h"

// Forward declarations
class Man;
class Bullet;

Ball::Ball (int t) {
    float tmp;

    size = BIG;
    type = t;

    pos[X] = RAND_DOMF (-SIZEX2 + 0.1f, SIZEX2 - 0.1f);
    pos[Y] = RAND_DOMF (0, SIZEY2 - 0.1f);
    pos[Z] = 0;

    rot[Z] = 0;
    rot[X] = 0;
    rot[Y] = 0;

    rspeed[X] = RAND_FRAC() * 0.5;
    rspeed[Y] = RAND_FRAC() * 0.5;
    rspeed[Z] = RAND_FRAC() * 0.5;

    tmp = 2 * (RAND_FRAC() - 0.5);
    tspeed[X] = tmp * BALL_SPEED;
    tspeed[Y] = 0;
    tspeed[Z] = 0;

    color[R] = 0.9;
    color[G] = 0.0;
    color[B] = 0.0;

    acc_ratio = 0.0005;

    //  printf("Mi no is %d",mynumber);
}

Ball::Ball (int t, int s, float px, float py) {
    float tmp;

    size = s;
    type = t;

    pos[X] = px;
    pos[Y] = py;
    pos[Z] = 0;

    rot[Z] = 0;
    rot[X] = 0;
    rot[Y] = 0;

    rspeed[X] = RAND_FRAC() * 0.5;
    rspeed[Y] = RAND_FRAC() * 0.5;
    rspeed[Z] = RAND_FRAC() * 0.5;

    tmp = 2 * (RAND_FRAC() - 0.5);
    tspeed[X] = tmp * BALL_SPEED;
    tspeed[Y] = 0;
    tspeed[Z] = 0;

    if (size == BIG) {
        color[R] = 0.9;
        color[G] = 0.0;
        color[B] = 0.0;
    }

    if (size == MEDIUM) {
        color[R] = 0.9;
        color[G] = 0.5;
        color[B] = 0.0;
    }

    if (size == SMALL) {
        color[R] = 0.9;
        color[G] = 0.9;
        color[B] = 0.0;
    }

    acc_ratio = 0.001;
}

void Ball::draw() {
    predraw();
    glutSolidSphere (size * 0.3, 9, 8);

    // printf("Drawing ball %gm %g\n",pos[X],pos[Y]);

    postdraw();
}

Ball *Ball::split() {
    Ball *tmp = new Ball (1, size - 1, pos[X], pos[Y]);
    size--;

    // Actualiza la velocidad de la bola original y la nueva bola
    float splitSpeedX = 0.05f;  // Velocidad hacia la izquierda/derecha
    float splitSpeedY = 0.01f;  // Rebote opcional hacia arriba

    // Actualiza el color de la bola original
    updateColor();

    // La bola original va a la izquierda
    tspeed[X] = -splitSpeedX;
    tspeed[Y] = splitSpeedY;

    // La nueva bola va a la derecha
    tmp->tspeed[X] = splitSpeedX;
    tmp->tspeed[Y] = splitSpeedY;

    return tmp;
}

float Ball::getSize() {
    return (0.3 * size);
}

void Ball::reposition() {
    pos[X] += 1 + 3 * RAND_FRAC();
    pos[Y] += 1 + 3 * RAND_FRAC();
}

void Ball::updateColor() {
    if (size == BIG) {
        color[R] = 0.9;
        color[G] = 0.0;
        color[B] = 0.0;
    }

    if (size == MEDIUM) {
        color[R] = 0.9;
        color[G] = 0.5;
        color[B] = 0.0;
    }

    if (size == SMALL) {
        color[R] = 0.9;
        color[G] = 0.9;
        color[B] = 0.0;
    }
}

float *Ball::getSpeed() {
    static float speed[3];
    speed[X] = tspeed[X];
    speed[Y] = tspeed[Y];
    speed[Z] = tspeed[Z];
    return speed;
}

void Ball::setSpeed (float speed[3]) {
    tspeed[X] = speed[X];
    tspeed[Y] = speed[Y];
    tspeed[Z] = speed[Z];
}

void Ball::setPos (float pos[3]) {
    this->pos[X] = pos[X];
    this->pos[Y] = pos[Y];
    this->pos[Z] = pos[Z];
}

float Ball::operator+ (Shape *other) {
    float otherPos[3];
    other->getPos (otherPos);
    float dist = mydistance (pos[X], pos[Y], otherPos[X], otherPos[Y]);
    return dist;
}
