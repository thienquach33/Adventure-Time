#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2D.h"

class Transform {
    public :
        double X, Y;
    public : 
        Transform(double x = 0, double y = 0) : X(x), Y(y) {}
    public : 
        inline void moveX(double x) { X += x; }      
        inline void moveY(double y) { Y += y; }
        inline void move(Vector2D v) { X += v.X, Y += v.Y; };
};

#endif // TRANSFORM_H