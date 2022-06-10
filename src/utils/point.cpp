#include "point.hpp"
#include "spherical_coord.hpp"
#include "cylindrical_coord.hpp"


#include <math.h> 


Point::Point(float a, float b, float c){
    x = a;
    y = b;
    z = c;
};

Point::Point(SphericalCoord sc) {
    x = sc.radius * cosf(sc.beta) * sinf(sc.alpha);
    y = sc.radius * sinf(sc.beta);
    z = sc.radius * cosf(sc.beta) * cosf(sc.alpha);
}

Point::Point(CylindricalCoord cc) {
    x = cc.radius * sinf(cc.alpha);
    y = cc.height;
    z = cc.radius * cosf(cc.alpha);
}


void Point::addVector(Vector vec) {
    x += vec.vx;
    y += vec.vy;
    z += vec.vz;

}

void Point::addPoint(Point p){
    x += p.x;
    y += p.y;
    z += p.z;
}