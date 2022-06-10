#ifndef LIGHT_H
#define LIGHT_H

#include <string>

#include "../utils/point.hpp"
#include "../utils/vector.hpp"

enum LightType {
    POINT,
    DIRECTIONAL,
    SPOTLIGHT
};

enum LightType lightTypeStringToEnum(std::string light_type);

class Light {
    public:
        LightType type;
        Point pos;
        Vector dir;
        float cut_off;


        Light();

        Light(LightType type, Point pos, Vector dir, float cut_off);

        std::vector<float> getSourceVectorf();
        std::vector<float> getSpotLightDirVectorf();
        
};

#endif
