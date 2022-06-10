#ifndef ROTATION_H
#define ROTATION_H

class Rotation {
public:
    float time, alpha, axis_x, axis_y, axis_z;
    Rotation(float time, float alpha, float axis_x, float axis_y, float axis_z);
    Rotation();
};

#endif
