#include "../utils/vector.hpp"
#include "../utils/point.hpp"
#include "../utils/point2D.hpp"
#include "plane.hpp"
#include <vector>
#include <tuple>

Plane::Plane(float s, float d){
    size = s;
    divisions = d;
}

// we choose to pass an origin point instead of doing the required math inside this function 
//because this function can be used to draw the box and there is no need to rewrite code
void Plane::drawPlane(std::vector<Point>& points, std::vector<Vector>& normals, std::vector<Point2D>& textures, Point origin, Vector horizontalVec, Vector verticalVec, Vector normal, bool flag){

    //starting a top-down, left-right approach since that is how computers draw in screen
    Point refp0 = Point(origin.x, origin.y, origin.z);
    Point refp1 = Point(refp0.x, refp0.y, refp0.z);
    refp1.addVector(horizontalVec);
    Point refp2 = Point(refp0.x, refp0.y, refp0.z);
    refp2.addVector(verticalVec);
    Point refp3 = Point(refp2.x, refp2.y, refp2.z); // maybe use the reference point instead of the second point?
    refp3.addVector(horizontalVec);

    for(int i = 0; i < divisions; ++i){
        Point p0 = Point(refp0.x, refp0.y, refp0.z);
        Point p1 = Point(refp1.x, refp1.y, refp1.z);
        Point p2 = Point(refp2.x, refp2.y, refp2.z);
        Point p3 = Point(refp3.x, refp3.y, refp3.z);

        for(int j = 0; j < divisions; ++j){

            if(!flag){
                textures.push_back(Point2D(j, divisions - i)); 
                textures.push_back(Point2D(j + 1, divisions - (i + 1)));
                textures.push_back(Point2D(j + 1, divisions - i));
                textures.push_back(Point2D(j, divisions - i));
                textures.push_back(Point2D(j, divisions - (i + 1))); 
                textures.push_back(Point2D(j + 1, divisions - (i + 1)));
            }else{
                textures.push_back(Point2D(j/divisions, 1 - (i/divisions))); 
                textures.push_back(Point2D((j+1) / divisions, 1 - ((i+1) / divisions)));
                textures.push_back(Point2D((j + 1) / divisions , 1 - (i/ divisions)));
                textures.push_back(Point2D(j/divisions, 1 - (i/divisions)));
                textures.push_back(Point2D(j/ divisions, 1 - ((i+1) / divisions)));
                textures.push_back(Point2D((j + 1)/divisions, 1 - ((i+1) / divisions)));
            }
            
            //first triangle(up)
            points.push_back(p0);
            points.push_back(p3);
            points.push_back(p1);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);

            //second triangle(down)
            points.push_back(p0);
            points.push_back(p2);
            points.push_back(p3);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);

            //translate points horizontally
            p0.addVector(horizontalVec);
            p1.addVector(horizontalVec);
            p2.addVector(horizontalVec);
            p3.addVector(horizontalVec);

        }

        //translate ref points vertically down
        refp0.addVector(verticalVec);
        refp1.addVector(verticalVec);
        refp2.addVector(verticalVec);
        refp3.addVector(verticalVec);
    }
}

std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> Plane::draw(){

    std::vector<Point> points;
    std::vector<Vector> normals;
    std::vector<Point2D> textures;

    float vectorSize = size / divisions;
    float half = size / 2;

    drawPlane(points, normals, textures, Point(-half, 0, -half), Vector(vectorSize, 0, 0), Vector(0, 0, vectorSize), Vector(0, 1, 0), false);

    return std::make_tuple(std::move(points), std::move(normals), std::move(textures));
}