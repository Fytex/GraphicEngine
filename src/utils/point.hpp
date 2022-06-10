#ifndef POINT_H
#define POINT_H

#include <stdexcept>

class Vector;
class SphericalCoord;
class CylindricalCoord;

class Point{
    public:
        float x, y, z;
        Point(float a = 0, float b = 0, float c = 0);
        Point(SphericalCoord sc);
        Point(CylindricalCoord cc);
        void addVector(Vector vec);
        void addPoint(Point p);

        float& operator[] (size_t i)
        {
            switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::invalid_argument("Out of bound point");
            }
        }

        float operator[] (size_t i) const
        {
            return (*const_cast<Point*>(this))[i];
        }
};

#endif
