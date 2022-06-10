#include <vector>

#include "color.hpp"

Color::Color() {

}

Color::Color(int R, int G, int B){
    this->R = R;
    this->G = G;
    this->B = B;
};

std::vector<float> Color::getVectorf(void) {
    return std::vector{ ((float) this->R) / 255, ((float) this->G) / 255, ((float) this->B) / 255, 1.0f };
}

