#include "commonstuff.h"
#include "Animal.h"

Animal::Animal() {
    pos[X] = RAND_DOM (-SIZEX2 + 0.1f, SIZEX2 - 0.1f);
    pos[Y] = SIZEX2;  // Se genera en la parte superior
    pos[Z] = 0;

    rot[Z] = 0;
    rot[X] = 0;
    rot[Y] = 0;

    rspeed[X] = 0;
    rspeed[Y] = 0;
    rspeed[Z] = 0;

    tspeed[X] = 0;
    tspeed[Y] = 0.25;  // Velocidad hacia abajo
    tspeed[Z] = 0;

    // Amarillo
    color[R] = 1.0;
    color[G] = 1.0;
    color[B] = 0.0;

    acc_ratio = 0.0;
}

void Animal::draw() {
    predraw();

    // Elipse
    glPushMatrix();
    glColor3f (color[R], color[G], color[B]);
    glScaled (1.0f, 2.0f, 1.0f);     // Escala para hacer el cuerpo más alargado
    glutSolidSphere (0.4f, 20, 20);  // Cuerpo del animal
    glPopMatrix();

    // Pico
    glPushMatrix();
    glColor3f (0.2f, 0.2f, 0.2f);         // Color del pico
    glTranslatef (0.0f, -0.8f, 0.0f);     // Posición del pico
    glRotatef (90.0f, 0.0f, 1.0f, 0.0f);  // Rotación para que quede en la posición correcta
    glRotatef (90.0f, 1.0f, 0.0f, 0.0f);  // Rotación para que quede en la posición correcta
    glutSolidCone (0.12f, 0.3f, 10, 10);  // Pico del animal
    glPopMatrix();

    // Alas
    glPushMatrix();
    glColor3f (0.8f, 0.8f, 0.0f);          // Color de las alas
    glTranslatef (0.1f, -0.1f, 0.0f);      // Posición de las alas
    glRotatef (90.0f, 0.0f, 1.0f, 0.0f);   // Rotación para que queden en la posición correcta
    glRotatef (-30.0f, 1.0f, 0.0f, 0.0f);  // Rotación para que queden en la posición correcta
    glutSolidCone (0.35f, 1.0f, 10, 10);   // Ala derecha
    glPopMatrix();

    glPushMatrix();
    glColor3f (0.8f, 0.8f, 0.0f);          // Color de las alas
    glTranslatef (-0.1f, -0.1f, 0.0f);     // Posición de las alas
    glRotatef (-90.0f, 0.0f, 1.0f, 0.0f);  // Rotación para que queden en la posición correcta
    glRotatef (-30.0f, 1.0f, 0.0f, 0.0f);  // Rotación para que queden en la posición correcta
    glutSolidCone (0.35f, 1.0f, 10, 10);   // Ala izquierda
    glPopMatrix();

    // Cola
    glPushMatrix();
    glColor3f (0.6f, 0.6f, 0.0f);         // Color de la cola
    glTranslatef (0.0f, 1.0f, 0.0f);      // Posición de la cola
    glRotatef (90.0f, 0.0f, 1.0f, 0.0f);  // Rotación para que quede en la posición correcta
    glRotatef (90.0f, 1.0f, 0.0f, 0.0f);  // Rotación para que quede en la posición correcta
    glutSolidCone (0.25f, 0.5f, 10, 10);  // Cola del animal
    glPopMatrix();

    // Fin del dibujo del animal

    postdraw();
}

float Animal::getSize() {
    return 0.6;  // Tamaño del animal
}

float Animal::operator+ (Shape *other) {
    float otherPos[3];
    other->getPos (otherPos);
    float dist = mydistance (pos[X], pos[Y], otherPos[X], otherPos[Y]);
    return dist;
}
// Implementación del operador + para calcular la distancia entre el animal y otro objeto