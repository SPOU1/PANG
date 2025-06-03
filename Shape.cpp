#include "Shape.h"

Shape::Shape() {
    mynumber = RAND_DOM (0, 1000000);
    // TODO: check mynumber is not repeated
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

// void Shape::move() {
//     pos[X] += tspeed[X];
//     pos[Y] += tspeed[Y];
//     pos[Z] += tspeed[Z];
//     rot[X] += rspeed[X];
//     rot[Y] += rspeed[Y];
//     rot[Z] += rspeed[Z];

//     // updates speed
//     tspeed[Y] += GRAV * acc_ratio;

//     // check world boundaries
//     if (pos[X] <= -SIZEX2) {
//         pos[X] = -SIZEX2;
//         tspeed[X] = -tspeed[X];
//     }
//     if (pos[Y] <= -SIZEY2) {
//         pos[Y] = -SIZEY2;
//         tspeed[Y] = 4 * BALL_SPEED * (RAND_FRAC() + 0.2);
//     }  // -tspeed[Y];}
//     // if(pos[Z]<=-SIZEZ2) {pos[Z] = SIZEZ2; tspeed[Z] = -tspeed[Z];}

//     if (pos[X] >= SIZEX2) {
//         pos[X] = SIZEX2;
//         tspeed[X] = -tspeed[X];
//     }
//     if (pos[Y] >= SIZEY2) {
//         pos[Y] = SIZEY2;
//         tspeed[Y] = -0.01 * BALL_SPEED;
//     }
//     // if(pos[Z]>=SIZEZ2) {pos[Z] = -SIZEX2; tspeed[Z] = -tspeed[Z];}

//     if (rot[X] > 360) rot[X] -= 360;
//     if (rot[Y] > 360) rot[Y] -= 360;
//     if (rot[Z] > 360) rot[Z] -= 360;

//     if (rot[X] < 0) rot[X] += 360;
//     if (rot[Y] < 0) rot[Y] += 360;
//     if (rot[Z] < 0) rot[Z] += 360;
// }

void Shape::move() {
    // Update position
    pos[X] += tspeed[X];
    pos[Y] += tspeed[Y];
    pos[Z] += tspeed[Z];

    // Update rotation
    rot[X] += rspeed[X];
    rot[Y] += rspeed[Y];
    rot[Z] += rspeed[Z];

    // Apply gravity
    tspeed[Y] += GRAV * acc_ratio;

    // --- Collision with floor (perfect elastic bounce) ---
    if (pos[Y] <= -SIZEY2) {
        pos[Y] = -SIZEY2;

        // Bounce up with full speed (no damping)
        tspeed[Y] = -tspeed[Y];
    }

    // --- Collision with ceiling ---
    if (pos[Y] >= SIZEY2) {
        pos[Y] = SIZEY2;
        tspeed[Y] = -tspeed[Y];
    }

    // --- Collision with sides ---
    if (pos[X] <= -SIZEX2) {
        pos[X] = -SIZEX2;
        tspeed[X] = -tspeed[X];
    }
    if (pos[X] >= SIZEX2) {
        pos[X] = SIZEX2;
        tspeed[X] = -tspeed[X];
    }

    // Optional Z axis bounce
    /*
    if (pos[Z] <= -SIZEZ2) {
        pos[Z] = -SIZEZ2;
        tspeed[Z] = -tspeed[Z];
    }
    if (pos[Z] >= SIZEZ2) {
        pos[Z] = SIZEZ2;
        tspeed[Z] = -tspeed[Z];
    }
    */

    // Clamp rotations to [0, 360]
    for (int i = 0; i < 3; i++) {
        if (rot[i] >= 360.0f) rot[i] -= 360.0f;
        if (rot[i] < 0.0f) rot[i] += 360.0f;
    }
}
