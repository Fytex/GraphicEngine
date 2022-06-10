#ifndef BOX_H
#define BOX_H

#include "../utils/point2D.hpp"
#include "../utils/point.hpp"
#include <vector>

class Box{    
    public:
        float size, divisions;
        Box(float size, float divisions);
        std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> draw();        
};

#endif
