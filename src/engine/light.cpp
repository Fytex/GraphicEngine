#include <string>
#include <vector>

#include "light.hpp"
#include "../utils/point.hpp"
#include "../utils/vector.hpp"

Light::Light() {

}

Light::Light(LightType type, Point pos, Vector dir, float cut_off) {
    this->type = type;
    this->pos = pos;
    this->dir = dir;
    this->cut_off = cut_off;
};

std::vector<float> Light::getSourceVectorf() {
    if (this->type == DIRECTIONAL)
        return std::vector{ dir.vx, dir.vy, dir.vz, 0.0f };
    else {
        return std::vector{ pos.x, pos.y, pos.z, 1.0f };
    }
}

std::vector<float> Light::getSpotLightDirVectorf() {
    if (this->type == SPOTLIGHT)
        return std::vector{ dir.vx, dir.vy, dir.vz, 0.0f };
    
    throw std::invalid_argument("Only SpotLight can call getSpotLightDirVectorf");
}



enum LightType lightTypeStringToEnum(std::string light_type) {
    if (light_type == "point")
        return LightType::POINT;

    if (light_type == "directional")
        return LightType::DIRECTIONAL;

    if (light_type == "spotlight")
        return LightType::SPOTLIGHT;

    throw std::invalid_argument("Unknown Light Type");
}

