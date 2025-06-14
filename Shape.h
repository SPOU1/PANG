#include "commonstuff.h"

// forward declaration
class ObjectList;

#ifndef __SHAPE_H__
    #define __SHAPE_H__

class Shape {
   protected:
    float pos[3];
    float rot[3];
    float color[3];
    float tspeed[3];
    float rspeed[3];
    float acc_ratio;
    int mynumber;

    // int detect_collision();
    void predraw();
    void postdraw();

   public:
    Shape();
    virtual void move();
    virtual void draw() = 0;
    void getPos (float *);
    virtual float getSize() = 0;
    int getNo();
};

#endif
