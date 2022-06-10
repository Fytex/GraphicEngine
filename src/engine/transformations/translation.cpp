#include <vector>

#include "translation.hpp"
#include "../../utils/point.hpp"

Translation::Translation(float x, float y, float z) {
    this->time = 0;
    this->x = x;
    this->y = y;
    this->z = z;
};

Translation::Translation(float time, bool align, std::vector<Point> points) {
    this->time = time;
    this->align = align;
    this->points = points;
}

Translation::Translation() : Translation(0, 0, 0) {

};