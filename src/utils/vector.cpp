#include "vector.hpp"
#include "point.hpp"

#include <math.h>

Vector::Vector() {

}

Vector::Vector(float va, float vb, float vc) {
    vx = va;
    vy = vb;
    vz = vc;
};

Vector::Vector(Point a, Point b) {
    vx = b.x - a.x;
    vy = b.y - a.y;
    vz = b.z - a.z;
}


void Vector::normalize(void) {
    float radius = sqrtf(powf(vx, 2) + powf(vy, 2) + powf(vz, 2));
    if (radius == 0) {
        vx = vy = vz = 0.0f;
    }
    else {
        vx /= radius;
        vy /= radius;
        vz /= radius;
    }
}

void Vector::multiply(float x) {
    vx *= x;
    vy *= x;
    vz *= x;
}

Vector Vector::cross(Vector vec) {
    return Vector(
        vy * vec.vz - vz * vec.vy,
        vz * vec.vx - vx * vec.vz,
        vx * vec.vy - vy * vec.vx
    );
}
