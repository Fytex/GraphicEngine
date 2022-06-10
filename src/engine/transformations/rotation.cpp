#include "rotation.hpp"

Rotation::Rotation(float time, float alpha, float axis_x, float axis_y, float axis_z) {
    this->time = time; // If time > 0 then alpha is ignored
    this->alpha = alpha;
    this->axis_x = axis_x;
    this->axis_y = axis_y;
    this->axis_z = axis_z;
};

Rotation::Rotation() : Rotation(0, 0, 0, 0, 0) {

};
