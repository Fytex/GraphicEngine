#ifndef CYLINDRIC_COORD_H
#define CYLINDRIC_COORD_H

#include "vector.hpp"

class CylindricalCoord{
    public:
        float alpha, height, radius;
        CylindricalCoord(float alpha = 0, float height = 0, float radius = 0);
        
};

#endif
