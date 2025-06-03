#include "commonstuff.h"
#include "Bullet.h"

Bullet::Bullet (float px, float py, float vx, float vy) {
    pos[X] = px;
    pos[Y] = py;
    pos[Z] = 0;

    rot[Z] = 0;
    rot[X] = -90;
    rot[Y] = 0;

    rspeed[X] = 0;
    rspeed[Y] = 0;
    rspeed[Z] = 0;

    tspeed[X] = vx;
    tspeed[Y] = vy;
    tspeed[Z] = 0;

    color[R] = 0.0;
    color[G] = 0.0;
    color[B] = 0.9;

    acc_ratio = 0.0;
}

void Bullet::draw() {
    predraw();
    glutSolidCone (0.15, 0.5, 10, 10);
    postdraw();
}

float Bullet::getSize() {
    return (0.1);
}
