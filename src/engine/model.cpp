#include "model.hpp"
#include <string>

Model::Model() {

}

void Model::init(std::string file, std::string texture,
    Color color_diffuse, Color color_ambient, Color color_specular,
    Color color_emissive, int color_shininess){
    this->file = file;
    this->texture = texture;
    this->color_diffuse = color_diffuse;
    this->color_ambient = color_ambient;
    this->color_specular = color_specular;
    this->color_emissive = color_emissive;
    this->color_shininess = color_shininess;
};

