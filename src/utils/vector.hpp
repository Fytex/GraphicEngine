#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>

#include "point.hpp"


class Vector{
    public:
        float vx, vy, vz;
        Vector();
        Vector(float vx, float vy, float vz);
        Vector(Point a, Point b);
        void normalize(void);
        void multiply(float x);
        Vector cross(Vector vec);

        float& operator[] (size_t i)
        {
            switch (i) {
            case 0: return vx;
            case 1: return vy;
            case 2: return vz;
            default: throw std::invalid_argument("Out of bound vector");
            }
        }

        float operator[] (size_t i) const
        {
            return (*const_cast<Vector*>(this))[i];
        }
        
};

#endif
