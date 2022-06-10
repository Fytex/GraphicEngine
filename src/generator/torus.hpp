#ifndef TORUS_H
#define TORUS_H

#include <vector>
#include "../utils/point.hpp"

class Torus{
    public:
        float radius_outer, radius_inner, slices, stacks;
        Torus(float radius_outer, float radius_inner, float slices, float stacks);
        std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> draw();
        
};

#endif
