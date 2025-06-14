#include "commonstuff.h"
#include "Shape.h"

#ifndef __OBSTACLE_H__
    #define __OBSTACLE_H__

class Obstacle: public Shape {
   private:
    float size[2];

   public:
    Obstacle (float pos[3], float size[2]);
    void draw();
    float getSize();
    float getDistanceToEdge (float theta);
    float operator+ (Shape *other);
};

#endif