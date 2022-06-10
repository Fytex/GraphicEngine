#include "../utils/vector.hpp"
#include "../utils/point2D.hpp"
#include "bezierpatches.hpp"

#define _USE_MATH_DEFINES
#include "math.h"

#include <tuple>
#include <fstream>
#include <string>
#include <iostream>    
#include <sstream>
#include <algorithm> 

static const float m[4][4] = {{-1.0f, +3.0f, -3.0f, +1.0f},
                    {+3.0f, -6.0f, +3.0f, +0.0f},
                    {-3.0f, +3.0f, +0.0f, +0.0f},
                    {+1.0f, +0.0f, +0.0f, +0.0f}};

Bezier::Bezier(int level){
    tess_level = level;    
}

void Bezier::parse(char *filename){
    std::ifstream file(filename);

    file >> num_patches;
    file.ignore(1,'\n');
    
    std::string line;
    int i = 0;
    while(i < num_patches && getline(file, line)){

        std::vector<int> ind;
        std::istringstream iline(line);
        std::string num;
        
        while(getline(iline, num, ',')){
            
            num.erase(remove_if(num.begin(), num.end(), isspace), num.end());
            ind.push_back(stoi(num));
        }
        ++i;
        indices.push_back(ind);
        
    }

    file >> num_points;
    file.ignore(1,'\n');

    int j = 0;        
    while(j < num_points && getline(file, line)){
        int i = 0;
        float points[3];

        std::istringstream iline(line);
        std::string num;
        while(getline(iline, num, ',')){
            num.erase(remove_if(num.begin(), num.end(), isspace), num.end());
            points[i] = stof(num);
            ++i;
        }       
        control_points.push_back(Point(points[0],points[1],points[2]));
        ++j;
    }
}

void Bezier::buildMatrix(std::vector<Point> &points, Point m[4][4]){

    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j)
            m[i][j] = points.at(i* 4 + j);
    }
}


void Bezier::multMat(const float m[4][4], Point m2[4][4], Point m3[4][4]){
    for(int i = 0; i < 4; ++i){
        for(int k = 0; k < 4; ++k){
            for(int j = 0; j < 4; ++j){
                Point p = m2[k][j];
                float value = m[i][k];
                m3[i][j].addPoint(Point(value * p.x, value * p.y, value * p.z));
            }
        }
    }
}

void Bezier::multMatPointMat(Point m[4][4], const float m2[4][4], Point m3[4][4]){
    for(int i = 0; i < 4; ++i){
        for(int k = 0; k < 4; ++k){
            for(int j = 0; j < 4; ++j){
                Point p = m[i][k];
                float value = m2[k][j];
                m3[i][j].addPoint(Point(value * p.x, value * p.y, value * p.z));
            }
        }
    }
}
void Bezier::multMatrixVector(Point m[4][4], float *v, Point *res){

	for (int j = 0; j < 4; ++j) {
		for (int k = 0; k < 4; ++k) {
            Point p = m[j][k];
            float value = v[k];
			res[j].addPoint(Point(value * p.x, value * p.y, value * p.z));
		}
	}
}

void Bezier::multVecMat(float *vec, Point m1[4][4], Point *res){

    for (int j = 0; j < 4; ++j) {
		for (int k = 0; k < 4; ++k) {
            Point p = m1[j][k];
            float val = vec[k];
			res[j].addPoint(Point(val * p.x, val * p.y, val * p.z));
		}
	}
}

Point Bezier::multVecVec(Point *vec, float m1[4]){

    Point res;
    
    for(int i = 0; i < 4; ++i){
        Point p = vec[i];
        float val = m1[i];
        res.addPoint(Point(p.x * val, p.y * val, p.z * val));
    }   
    
    return res;

}

void Bezier::multPointVecMat(Point *vec, float m1[4][4], Point *res){

   for (int j = 0; j < 4; ++j) {
		res[j] = Point(0,0,0);
		for (int k = 0; k < 4; ++k) {
            auto val = m1[j][k];
            Point p = vec[k];
			res[j].addPoint(Point(val * p.x, val * p.y, val * p.z));
		}
	}
}

void Bezier::computeMatrix(std::vector<Point> &points, Point a[4][4]){

    Point matrixPoints[4][4];
    buildMatrix(points, matrixPoints);

    Point firstM[4][4];  
    multMat(m, matrixPoints, firstM);

    multMatPointMat(firstM, m, a);    
}

std::tuple<Point, Vector> Bezier::getBezierPoint(float u, float v, Point matrix[4][4]){
    
    
    float uvec[4] = {powf(u,3.0f), powf(u,2.0f), u, 1.0f};
    float vvec[4] = {powf(v,3.0f), powf(v,2.0f), v, 1.0f};
    float derivu[4] = {3.0f * powf(u,2.0f), 2.0f * u, 1, 0};
    float derivv[4] = {3.0f * powf(v,2.0f), 2.0f * v, 1, 0};

    Point firstDeriv[4];
    multMatrixVector(matrix, derivu, firstDeriv);
    Point derivUPoint = multVecVec(firstDeriv, vvec);
    Vector vectorU(derivUPoint.x, derivUPoint.y, derivUPoint.z);

    Point secDeriv[4];
    multMatrixVector(matrix, uvec, secDeriv);
    Point derivVPoint = multVecVec(secDeriv, derivv);
    Vector vectorV(derivVPoint.x, derivVPoint.y, derivVPoint.z);

    Vector normal = vectorU.cross(vectorV);
    normal.normalize();

    Point firstVec[4];
    multMatrixVector(matrix, uvec, firstVec);    
    
    return std::make_tuple(multVecVec(firstVec, vvec), normal);

}

std::vector<Point> Bezier::getPatchPoints(int patchNumber){

    std::vector<Point> res;
    std::vector<int> patchPoints = indices[patchNumber];


    for(size_t i = 0; i < patchPoints.size(); ++i)
        res.push_back(control_points[patchPoints[i]]);

    return res;

}

std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> Bezier::draw(){

    std::vector<Point> points;
    std::vector<Vector> normals;
    std::vector<Point2D> textures;

    float alpha = 1.0f / tess_level;

    for(int i = 0; i < num_patches; ++i){

        std::vector<Point> patchPoints = getPatchPoints(i);
        Point matrix[4][4];
        computeMatrix(patchPoints, matrix);

        for(int j = 0; j < tess_level; ++j){

            float u = j * alpha;
            float nextU = (j+ 1) * alpha;


            for (int k = 0; k < tess_level; ++k) {
            
                float v = k * alpha;
                float nextV = (k + 1) * alpha;

                
                auto [p0, n0] = getBezierPoint(u, v, matrix);
                auto [p1, n1] = getBezierPoint(nextU, v, matrix);
                auto [p2, n2] = getBezierPoint(nextU, nextV, matrix);
                auto [p3, n3] = getBezierPoint(u, nextV, matrix);
                 
                points.push_back(p0);
                points.push_back(p1);
                points.push_back(p2);

                normals.push_back(n0);
                normals.push_back(n1);
                normals.push_back(n2);

                textures.push_back(Point2D(u, v));
                textures.push_back(Point2D(nextU, v));
                textures.push_back(Point2D(nextU, nextV));               

                points.push_back(p2);
                points.push_back(p3);
                points.push_back(p0);

                normals.push_back(n2);
                normals.push_back(n3);
                normals.push_back(n0);
                
                textures.push_back(Point2D(nextU, nextV));
                textures.push_back(Point2D(u, nextV));
                textures.push_back(Point2D(u, v));        
            
            }
        }
    }

    return std::make_tuple(std::move(points), std::move(normals), std::move(textures));

}
