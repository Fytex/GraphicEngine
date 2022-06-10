#ifndef CAMERA_H
#define CAMERA_H

#include "../utils/point.hpp"
#include "../utils/vector.hpp"


class Camera{
    public:
        Point eye, center;
        Vector up;
        double fov, near, far;
        Camera();
        void init(Point eye, Point center, Vector up, double fov, double near, double far);
        
};

#endif
