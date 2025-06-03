#include "commonstuff.h"
#include "Man.h"
#include "Bullet.h"
#include "Shape.h"
#include "Ball.h"
#include "Animal.h"

struct nodo {
    Shape *obj;
    struct nodo *next;
};

typedef struct nodo nodo;
typedef nodo *pnodo;

class ObjectsList {
   private:
    int n;
    nodo *head;
    Man *theMan;

   public:
    ObjectsList();
    ~ObjectsList();
    void move();
    void draw();
    Man *getMan();
    void add (Shape *s);
    void remove (Shape *s);
    void reposition (Man *TheMan);
    float collisions (Bullet *TheBullet, Man *TheMan, Animal *TheAnimal);
    void removeAllExceptMan (Man *TheMan);
    void createBalls();
};