#ifndef BEZIERPATCHES_H
#define BEZIERPATCHES_H

#include "../utils/point.hpp"
#include <vector>

class Bezier{
    public:
        Bezier(int level);
        
        int num_patches, tess_level, num_points;

        std::vector<std::vector<int>> indices;
        std::vector<Point> control_points;

        void parse(char *filename);

        void buildMatrix(std::vector<Point> &points, Point m[4][4]);

        void multVecMat(float *vec, Point m1[4][4], Point *res);
        Point multVecVec(Point *vec, float m1[4]);
        void multPointVecMat(Point *vec, float m1[4][4], Point *res);
        void multMatrixVector(Point m[4][4], float *v, Point *res);
        void multMat(const float m[4][4], Point m2[4][4], Point m3[4][4]);
        void multMatPointMat(Point m[4][4], const float m2[4][4], Point m3[4][4]);

        void computeMatrix(std::vector<Point> &points, Point a[4][4]);
        std::vector<Point> getPatchPoints(int patchNumber);

        std::tuple<Point, Vector> getBezierPoint(float u, float v, Point matrix[4][4]);

        std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> draw();

};

#endif
