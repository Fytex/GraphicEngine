#include "spherical_coord.hpp"

#define _USE_MATH_DEFINES
#include <math.h>



SphericalCoord::SphericalCoord(float alp, float bet, float r){
    alpha = alp;
    beta = bet;
    radius = r;
};

#include <iostream>
SphericalCoord::SphericalCoord(Point point) {
    radius = sqrtf(powf(point.x, 2) + powf(point.y, 2) + powf(point.z, 2));
    beta = (radius == 0) ? 0 : asinf(point.y / radius);
    alpha = atan2f(point.x, point.z);
};
