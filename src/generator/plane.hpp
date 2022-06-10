#ifndef PLANE_H
#define PLANE_H

#include "../utils/vector.hpp"
#include "../utils/point.hpp"
#include "../utils/point2D.hpp"
#include <vector>
#include <tuple>

class Plane{    
    public:
        float size, divisions;
        Plane(float size, float divisions);
        void drawPlane(std::vector<Point>&, std::vector<Vector>&, std::vector<Point2D>&, Point origin, Vector horizontalVec, Vector verticalVec, Vector normal, bool flag);
        std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> draw();        
};

#endif