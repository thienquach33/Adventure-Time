#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

class Vector2D {
    public :
        int X, Y;
    public : 
        Vector2D(double x = 0, double y = 0) : X(x), Y(y) {}
    public : 
        inline Vector2D operator + (const Vector2D& v2) const {
            return Vector2D(X + v2.X, Y + v2.Y);
        }
        inline Vector2D operator - (const Vector2D &v2) const {
            return Vector2D(X - v2.X, Y - v2.Y);
        }
        inline Vector2D operator * (const double scale) const {
            return Vector2D(X * scale, Y * scale);
        }
};

#endif // VECTOR2D_H