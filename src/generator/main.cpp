#include "../utils/point.hpp"
#include "../utils/point2D.hpp"
#include "bezierpatches.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "torus.hpp"
#include "cone.hpp"
#include "box.hpp"

#include <any>
#include <tuple>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

void writeToFile(auto tuple, std::string filename){
    std::ofstream file(filename);
    std::ofstream filetex(filename + ".tex");
    auto [points, normals, textures] = tuple;
    file << points.size() << std::endl;
    
    for(auto i = 0; i < points.size(); ++i){
        file << points.at(i).x << " " << points.at(i).y << " " << points.at(i).z << " " 
                << normals.at(i).vx << " " << normals.at(i).vy << " " << normals.at(i).vz << " " << std::endl;
        filetex << textures.at(i).x << " " << textures.at(i).y << std::endl;    
    }

    file.close();

}

int main(int argc, char **argv) {

    if(argc < 2) {
        std::cout << "Necessita de pelo menos dois argumentos" << std::endl;
        return 1;
    }

    std::tuple<std::vector<Point>, std::vector<Vector>, std::vector<Point2D>> tuple;
    std::vector<Point> points;
    std::string figure(argv[1]);
    std::string file;
    bool flag = true;

    if(!figure.compare("plane")){
        if(argc == 5){
            float size = atof(argv[2]);
            float divisions = atof(argv[3]);
            file = argv[4];

            Plane plane(size, divisions);
            tuple = plane.draw();
                                
        }else{
            flag = false;
            std::cout << "Plane necessita de 3 argumentos" << std::endl;
        }
    }else if(!figure.compare("box")){
        if(argc == 5){
            float size = atof(argv[2]);
            float divisions = atof(argv[3]);
            file = argv[4];

            Box box(size, divisions);
            tuple = box.draw();
                                
        }else{
            flag = false;
            std::cout << "Box necessita de 3 argumentos" << std::endl;
        }
    }else if(!figure.compare("cone")){ 
        if(argc == 7){
            
            float radius = atof(argv[2]);
            float height = atof(argv[3]);
            float slices = atof(argv[4]);
            float stacks = atof(argv[5]);
            file = argv[6];

            Cone cone(radius, height, slices, stacks);
            tuple = cone.draw();
            
        }else{
            flag = false;
            std::cout << "Cone necessita de 5 argumentos" << std::endl;
        }
    }else if(!figure.compare("sphere")){
        if(argc == 6){

            float radius = atof(argv[2]);
            float slices = atof(argv[3]);
            float stacks = atof(argv[4]);
            file = argv[5];

            Sphere sphere(radius, slices, stacks);
            tuple = sphere.draw();
        }else{
            flag = false;
            std::cout << "Sphere necessita de 4 argumentos" << std::endl;
        }

    }
    else if (!figure.compare("torus")) {
        if (argc == 7) {

            float radius_outer = atof(argv[2]);
            float radius_inner = atof(argv[3]);
            float slices = atof(argv[4]);
            float stacks = atof(argv[5]);
            file = argv[6];

            Torus torus(radius_outer, radius_inner, slices, stacks);
            tuple = torus.draw();
        }
        else {
            flag = false;
            std::cout << "Torus necessita de 5 argumentos" << std::endl;
        }

    }
    else if(!figure.compare("bezier")){
        if(argc == 5){

            char * filein = argv[2];
            int level = atoi(argv[3]);
            file = argv[4];

            Bezier bezier(level);
            bezier.parse(filein);

            tuple = bezier.draw();

        }else{
            flag = false;
            std::cout << "Bezier necessita de 3 argumentos" << std::endl;
        }

    }else{
        flag = false;
        std::cout << "Comando nao disponivel" << std::endl;
    }
    
    if(flag)
        writeToFile(tuple, file);

    return 0;
    
}

