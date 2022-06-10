#include "group.hpp"

#include <vector>
#include <stdexcept>

enum Transformation transformationStringToEnum(std::string transformation) {
	if (transformation == "translate")
		return Transformation::TRANSLATION;

	if (transformation == "rotate")
		return Transformation::ROTATION;

	if (transformation == "scale")
		return Transformation::SCALE;
	
	throw std::invalid_argument("Unknown Transformation");
}

Group::Group(){
};

void Group::add_translation(Translation translation) {
	this->translation = translation;

	this->ordered_transformations.push_back(Transformation::TRANSLATION);
}
void Group::add_rotation(Rotation rotation) {
	this->rotation = rotation;

	this->ordered_transformations.push_back(Transformation::ROTATION);
}
void Group::add_scale(Scale scale) {
	this->scale = scale;

	this->ordered_transformations.push_back(Transformation::SCALE);
}

void Group::add_model(Model &model) {
	this->models.push_back(model);
}

void Group::add_group(Group& group) {
	this->groups.push_back(group);
}
