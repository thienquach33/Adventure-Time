#ifndef POINT_H
#define POINT_H

class Point {
    public :    
        double x, y;
    public : 
        Point(double X = 0, double Y = 0) : x(X), y(Y) {}

        inline Point operator + (const Point& p2) const {
            return Point(x + p2.x, y + p2.y);
        }

        inline friend Point operator += (Point& p1, const Point& p2) {
            p1.x += p2.x;
            p1.y += p2.y;
            return p1;
        }

        inline Point operator - (const Point& p2) const {
            return Point(x - p2.x, y - p2.y);
        }

        inline friend Point operator -= (Point& p1, const Point& p2) {
            p1.x -= p2.x;
            p1.y -= p2.y;
            return p1;
        }

        inline Point operator * (const double scale) const {
            return Point(x * scale, y * scale);
        }
};

#endif // POINT_H