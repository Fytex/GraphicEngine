#ifndef GROUP_H
#define GROUP_H


#include "transformations/translation.hpp"
#include "transformations/rotation.hpp"
#include "transformations/scale.hpp"

#include "model.hpp"

#include <vector>

enum Transformation {
    TRANSLATION,
    ROTATION,
    SCALE
};

enum Transformation transformationStringToEnum(std::string transformation);

class Group{
    public:
        Translation translation;
        Rotation rotation;
        Scale scale;
        std::vector<Transformation> ordered_transformations;

        std::vector<Model> models;

        std::vector<Group> groups;

        Group();

        void add_translation(Translation translation);
        void add_rotation(Rotation rotation);
        void add_scale(Scale scale);

        void add_model(Model& model);

        void add_group(Group& group);
        
};

#endif
