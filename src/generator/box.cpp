#include "box.hpp"
#include "plane.hpp"

Box::Box(float s, float div){
    size = s;
    divisions = div;
}

std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> Box::draw(){
    std::vector<Point> points;
    std::vector<Vector> normals;
    std::vector<Point2D> textures;

    Plane plane(size, divisions); //no need to create a plane for each face, we use the same one and change the origin point

    float vectorSize = size / divisions;
    float half = size /2;
    

    //following top-down, left-right approach like before
    //upper face
    plane.drawPlane(points, normals, textures, Point(-half, half, -half), Vector(vectorSize, 0.0f , 0.0f), Vector(0.0f, 0.0f, vectorSize), Vector(0, 1, 0), true);
    
    //back face
    plane.drawPlane(points, normals, textures, Point(-half, half, -half), Vector(0.0f, 0.0f, vectorSize), Vector(0.0f, -vectorSize, 0.0f), Vector(-1, 0, 0), true);

    //left face
    plane.drawPlane(points, normals, textures, Point(-half, half, half), Vector(vectorSize, 0.0f, 0.0f), Vector(0.0f, -vectorSize, 0.0f), Vector(0, 0, 1), true);

    //rigth face
    plane.drawPlane(points, normals, textures, Point(half, half, -half), Vector(-vectorSize, 0.0f, 0.0f), Vector(0.0f, -vectorSize, 0.0f), Vector(0, 0, -1), true);

    //front face(need to start bottom-up to verify right hand rule)
    plane.drawPlane(points, normals, textures, Point(half, -half, -half), Vector(0.0f, 0.0f, vectorSize), Vector(0.0f, vectorSize, 0.0f), Vector(1, 0, 0), true);

    //bottom face
    plane.drawPlane(points, normals, textures, Point(-half, -half, half), Vector(vectorSize, 0.0f, 0.0f), Vector(0.0f, 0.0f, -vectorSize), Vector(0, -1, 0), true);

    return std::make_tuple(std::move(points), std::move(normals), std::move(textures));

}