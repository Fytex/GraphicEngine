#include "../utils/point.hpp"
#include "../utils/point2D.hpp"
#include "../utils/spherical_coord.hpp"
#include "sphere.hpp"

#include <vector>
#include <tuple>
#include <iostream>

#define _USE_MATH_DEFINES
#include "math.h"

Sphere::Sphere(float r, float sl, float st){
    radius = r;
    slices = sl;
    stacks = st;
}

std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> Sphere::draw(){

    auto alpha = 2*M_PI / slices;
    auto beta = M_PI / stacks;

    Point centerHigh(0, radius, 0);
    Point centerDown(0,-radius, 0);

    std::vector<Point> points;
    std::vector<Vector> normals;
    std::vector<Point2D> textures;

    //draw base and then proceed to "lateral"
    for(int i = 0; i < slices; ++i){
        //top base only need one triangle since the other would be in the same points
        Point p1(SphericalCoord(alpha * i, M_PI / 2 - beta, radius));
        Point p2(SphericalCoord(alpha * (i + 1), M_PI / 2 - beta, radius));

        points.push_back(p1);
        points.push_back(p2);
        points.push_back(centerHigh);

        normals.push_back(Vector(p1.x/radius, p1.y/radius, p1.z/radius));
        normals.push_back(Vector(p2.x/radius, p2.y/radius, p2.z/radius));
        normals.push_back(Vector(centerHigh.x/radius, centerHigh.y/radius, centerHigh.z/radius));

        textures.push_back(Point2D((i * alpha)/(2 * M_PI) , 1 - beta / M_PI));
        textures.push_back(Point2D(((i + 1) * alpha)/(2 * M_PI) , 1 - beta / M_PI));
        textures.push_back(Point2D((i + 0.5) * alpha / (2 * M_PI), 1));

        for(int j = 1; j < stacks; ++j){
            //starting up-down
            Point pointDownL(SphericalCoord(alpha * i, M_PI / 2 - beta * (j + 1), radius));
            Point pointDownR(SphericalCoord(alpha * (i + 1), M_PI / 2 - beta * (j + 1), radius));
            Point pointL(SphericalCoord(alpha * i, M_PI / 2 - beta * j, radius));
            Point pointR(SphericalCoord(alpha * (i + 1), M_PI / 2 - beta * j, radius));
            
            //last stack is bottom base
            if(j == stacks - 1){
                points.push_back(centerDown);
                points.push_back(pointR);
                points.push_back(pointL);
                
                normals.push_back(Vector(centerDown.x/radius, centerDown.y/radius, centerDown.z/radius));
                normals.push_back(Vector(pointR.x/radius, pointR.y/radius, pointR.z/radius));
                normals.push_back(Vector(pointL.x/radius, pointL.y/radius, pointL.z/radius));

                textures.push_back(Point2D(((i + 0.5) * alpha)/(2 * M_PI) , 0));
                textures.push_back(Point2D(((i + 1) * alpha)/(2 * M_PI) , beta / M_PI));
                textures.push_back(Point2D(i * alpha / (2 * M_PI), beta / M_PI));
                
            }else{
                points.push_back(pointDownL);
                points.push_back(pointDownR);
                points.push_back(pointL);

                normals.push_back(Vector(pointDownL.x/radius, pointDownL.y/radius, pointDownL.z/radius));
                normals.push_back(Vector(pointDownR.x/radius, pointDownR.y/radius, pointDownR.z/radius));
                normals.push_back(Vector(pointL.x/radius, pointL.y/radius, pointL.z/radius));

                textures.push_back(Point2D((i * alpha)/(2 * M_PI) , 1 - ((j + 1) * beta)  /M_PI));
                textures.push_back(Point2D(((i + 1) * alpha)/(2 * M_PI) , 1 - ((j + 1) * beta) / M_PI));
                textures.push_back(Point2D(i * alpha / (2 * M_PI), 1 - (j * beta) / M_PI));

                points.push_back(pointDownR);
                points.push_back(pointR);
                points.push_back(pointL);

                normals.push_back(Vector(pointDownR.x/radius, pointDownR.y/radius, pointDownR.z/radius));
                normals.push_back(Vector(pointR.x/radius, pointR.y/radius, pointR.z/radius));
                normals.push_back(Vector(pointL.x/radius, pointL.y/radius, pointL.z/radius));

                textures.push_back(Point2D(((i + 1) * alpha)/(2 * M_PI) , 1 - ((j + 1)  * beta) /M_PI));
                textures.push_back(Point2D(((i + 1) * alpha)/(2 * M_PI) , 1 - (j * beta) / M_PI));
                textures.push_back(Point2D(i * alpha / (2 * M_PI), 1 - (j * beta)  / M_PI));
            }        
        }
    }

    return std::make_tuple(std::move(points), std::move(normals), std::move(textures));

}