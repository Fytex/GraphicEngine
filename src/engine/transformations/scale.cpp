#include "scale.hpp"

Scale::Scale(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
};

Scale::Scale() : Scale(1, 1, 1) {

};

