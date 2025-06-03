#include <time.h>
#include "ObjectsList.h"
#include "Ball.h"
#include "Animal.h"

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

    // No hay colisión
    return 0;
}

void ObjectsList::removeAllExceptMan (Man *TheMan) {
    nodo *node = head;
    nodo *prev = NULL;

    while (node != NULL) {
        if (node->obj != TheMan) {
            nodo *temp = node;  // Save current node to delete later

            // Update pointers to skip this node
            if (prev == NULL) {
                head = node->next;
            } else {
                prev->next = node->next;
            }

            // Move to next node before deletion
            node = node->next;

            // Delete the saved node
            delete temp;
            n--;
        } else {
            // Keep this node, move to next
            prev = node;
            node = node->next;
        }
    }
}

void ObjectsList::createBalls() {
    // Inicializa NUMBALLS bolas
    for (int i = 0; i < NUMBALLS; i++) {
        Ball *theBall = new Ball (1);
        add (theBall);
    }
}