#ifndef CATMULL_ROM_H
#define CATMULL_ROM_H

#include <tuple>

#include "../utils/point.hpp"
#include "../utils/vector.hpp"

void buildRotMatrix(Vector x, Vector y, Vector z, float* m);

std::tuple<Point, Vector> getGlobalCatmullRomPoint(std::vector<Point> points, float gt);

#endif