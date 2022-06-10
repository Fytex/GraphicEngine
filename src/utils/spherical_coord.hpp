#ifndef SPHERICAL_COORD_H
#define SPHERICAL_COORD_H

#include "vector.hpp"
#include "point.hpp"

class SphericalCoord{
    public:
        float alpha, beta, radius;
        SphericalCoord(float alp = 0, float bet = 0, float r = 0);
        SphericalCoord(Point point);
        
};

#endif
