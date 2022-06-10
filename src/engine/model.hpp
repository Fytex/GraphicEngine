#ifndef MODEL_H
#define MODEL_H

#include <string>

#include "color.hpp"


class Model{
    public:
        std::string file, texture;
        Color color_diffuse, color_ambient, color_specular, color_emissive;
        int color_shininess;


        Model();

        void init(std::string file, std::string texture, 
            Color color_diffuse, Color color_ambient, Color color_specular, 
            Color color_emissive, int color_shininess);
        
};

#endif
