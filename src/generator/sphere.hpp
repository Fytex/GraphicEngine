#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "../utils/point.hpp"

class Sphere{
    public:
        float radius, slices, stacks;
        Sphere(float radius, float sices, float stacks);
        std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> draw();
        
};

#endif
