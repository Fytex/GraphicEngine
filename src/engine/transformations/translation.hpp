#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <vector>

#include "../../utils/point.hpp"

class Translation {
public:
    bool align;
    float time, x, y, z;
    std::vector<Point> points;
    Translation(float x, float y, float z);
    Translation(float time, bool align, std::vector<Point> points);
    Translation();
};

#endif