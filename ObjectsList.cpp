#include <time.h>
#include "ObjectsList.h"
#include "Ball.h"
#include "Animal.h"
#include "Obstacle.h"

ObjectsList::ObjectsList() {
    n = 0;
    head = NULL;
    theMan = NULL;

    // Inicializa el jugador
    theMan = new Man();
    add (theMan);

    // Randomiza la semilla
    SEED_RAND (time (NULL));
}

ObjectsList::~ObjectsList() {
    nodo *node = head;
    nodo *tmp;
    while (node != NULL) {
        tmp = node;
        node = node->next;
        delete tmp;
    }
}

void ObjectsList::move() {
    nodo *node = head;
    while (node != NULL) {
        node->obj->move();
        node = node->next;
    }
}

void ObjectsList::draw() {
    nodo *node = head;
    while (node != NULL) {
        node->obj->draw();
        node = node->next;
    }
}

Man *ObjectsList::getMan() {
    return theMan;
}

void ObjectsList::add (Shape *s) {
    nodo *newNode = new nodo;
    newNode->obj = s;
    if (head == NULL) {  // La lista está vacía
        newNode->next = NULL;
    } else {  // La lista no está vacía
        newNode->next = head;
    }
    head = newNode;  // El nuevo nodo se convierte en la cabeza de la lista
    n++;
}

void ObjectsList::remove (Shape *s) {
    nodo *node = head;
    nodo *prev = NULL;

    while (node != NULL) {
        if (node->obj == s) {
            if (prev == NULL) {  // El nodo a eliminar es la cabeza
                head = node->next;
            } else {
                prev->next = node->next;
            }
            delete node;
            n--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

void ObjectsList::reposition (Man *theMan) {
    // theMan->resetpos();
    nodo *node = head;
    while (node != NULL) {
        if (Ball *theBall = dynamic_cast<Ball *> (node->obj)) {
            if (*theBall + theMan < theBall->getSize() + theMan->getSize() + 1) {  // Ball y Man
                theBall->reposition();
            }
        }
        node = node->next;
    }
}

float ObjectsList::collisions (Bullet *theBullet, Man *theMan, Animal *theAnimal) {
    // --- 1) Bullet → Ball  ---
    if (theBullet) {
        nodo *node = head;
        while (node) {
            Ball *theBall = dynamic_cast<Ball *> (node->obj);
            if (theBall) {
                float distBB = *theBall + theBullet;
                float sumR = theBall->getSize() + theBullet->getSize();

                if (distBB < sumR) {
                    float r = theBall->getSize();

                    // SMALL
                    if (r <= 0.3f + 1e-6f) {
                        remove (theBall);
                        return 4;
                    }
                    // MEDIUM
                    else if (r <= 0.6f + 1e-6f) {
                        Ball *newBall = theBall->split();  // crea dos bolas pequeñas
                        add (newBall);
                        return 2;
                    }
                    // BIG
                    else {
                        Ball *newBall = theBall->split();  // crea dos bolas medianas
                        add (newBall);
                        return 3;
                    }
                }
            }
            node = node->next;
        }
    }

    // --- 2) Ball → theMan ---
    {
        nodo *node = head;
        while (node) {
            Ball *theBall = dynamic_cast<Ball *> (node->obj);
            if (theBall) {
                float distBP = *theBall + theMan;
                float sumR = theBall->getSize() + theMan->getSize();
                if (distBP < sumR) {  // Colisión entre la bola y el jugador
                    return 1;
                }
            }
            node = node->next;
        }
    }

    // --- 3) Animal → Bullet ---
    if (theAnimal && theBullet) {
        float distAB = *theAnimal + theBullet;
        float sumR = theAnimal->getSize() + theBullet->getSize();
        if (distAB < sumR) {
            return 5;  // Colisión entre el animal y el proyectil
        }
    }

    // --- 4) Obstacle → Bullet ---
    if (theBullet) {
        nodo *node = head;
        while (node) {
            Obstacle *theObstacle = dynamic_cast<Obstacle *> (node->obj);
            if (theObstacle) {
                float distOB = *theObstacle + theBullet;
                float ObstaclePos[3];
                theObstacle->getPos (ObstaclePos);
                float BulletPos[3];
                theBullet->getPos (BulletPos);

                // Calcula la distancia al borde del obstáculo en la dirección del proyectil
                float theta = atan2 (BulletPos[Y] - ObstaclePos[Y], BulletPos[X] - ObstaclePos[X]);
                float distanceToEdge = theObstacle->getDistanceToEdge (theta);
                if (distOB < distanceToEdge + theBullet->getSize()) {
                    return 6;  // Colisión entre el obstáculo y el proyectil
                }
            }
            node = node->next;
        }
    }

    // --- 5) Obstacle → Ball ---
    {
        nodo *node = head;
        while (node) {
            Obstacle *theObstacle = dynamic_cast<Obstacle *> (node->obj);
            if (theObstacle) {
                nodo *node2 = head;
                while (node2) {
                    Ball *theBall = dynamic_cast<Ball *> (node2->obj);
                    if (theBall) {
                        float distOB = *theObstacle + theBall;
                        float ObstaclePos[3];
                        theObstacle->getPos (ObstaclePos);
                        float BallPos[3];
                        theBall->getPos (BallPos);

                        // Calcula la distancia al borde del obstáculo en la dirección de la bola
                        float theta =
                            atan2 (BallPos[Y] - ObstaclePos[Y], BallPos[X] - ObstaclePos[X]);
                        float distanceToEdge = theObstacle->getDistanceToEdge (theta);
                        if (distOB < distanceToEdge + theBall->getSize()) {
                            float newSpeed[3] = {0, 0, 0};
                            float normalX = cos (theta);
                            float normalY = sin (theta);
                            float dot =
                                theBall->getSpeed()[X] * normalX + theBall->getSpeed()[Y] * normalY;

                            newSpeed[X] = theBall->getSpeed()[X] - 2 * dot * normalX;
                            newSpeed[Y] = theBall->getSpeed()[Y] - 2 * dot * normalY;
                            theBall->setSpeed (newSpeed);  // Actualiza la velocidad de la bola

                            float overlap = distanceToEdge + theBall->getSize() - distOB;

                            // Reposiciona la bola para evitar colisión
                            float newPos[3] = {0, 0, 0};
                            newPos[X] = BallPos[X] + overlap * normalX;
                            newPos[Y] = BallPos[Y] + overlap * normalY;
                            theBall->setPos (newPos);

                            return 7;  // Colisión entre la bola y el obstáculo
                        }
                    }
                    node2 = node2->next;
                }
            }
            node = node->next;
        }
    }

    // No hay colisión
    return 0;
}

void ObjectsList::removeAllExceptMan (Man *TheMan) {
    nodo *node = head;
    nodo *prev = NULL;

    while (node != NULL) {
        if (node->obj != TheMan) {
            nodo *temp = node;  // Guardar el nodo a eliminar

            // Actualizar punteros para omitir este nodo
            if (prev == NULL) {
                head = node->next;
            } else {
                prev->next = node->next;
            }

            // Mover al siguiente nodo antes de la eliminación
            node = node->next;

            // Eliminar el nodo
            delete temp;
            n--;
        } else {
            // Guardar el nodo actual y avanzar
            prev = node;
            node = node->next;
        }
    }
}

void ObjectsList::createObstacles() {
    // Crea 4 obstáculos
    float pos1[3] = {4.0, 4.0, 0};
    float size1[2] = {2, 1};
    Obstacle *obstacle1 = new Obstacle (pos1, size1);
    add (obstacle1);

    float pos2[3] = {-4.0, 4.0, 0};
    float size2[2] = {2, 1};
    Obstacle *obstacle2 = new Obstacle (pos2, size2);
    add (obstacle2);

    float pos3[3] = {0.0, 4.0, 0};
    float size3[2] = {2, 1};
    Obstacle *obstacle3 = new Obstacle (pos3, size3);
    add (obstacle3);

    float pos4[3] = {0.0, -1.0, 0};
    float size4[2] = {2, 1};
    Obstacle *obstacle4 = new Obstacle (pos4, size4);
    add (obstacle4);
}

void ObjectsList::createBalls() {
    // Inicializa NUMBALLS bolas
    for (int i = 0; i < NUMBALLS; i++) {
        Ball *theBall = new Ball (1);
        add (theBall);
    }
}