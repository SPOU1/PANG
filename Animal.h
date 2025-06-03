#include "commonstuff.h"
#include "Shape.h"

#ifndef __ANIMAL_H__
    #define __ANIMAL_H__

class Animal: public Shape {
   public:
    Animal();
    void draw();
    float getSize();
    float operator+ (Shape *);
};

#endif