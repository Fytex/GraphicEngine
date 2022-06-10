#include "camera.hpp"
#include "../utils/point.hpp"

Camera::Camera() {
}

void Camera::init(Point eye, Point center, Vector up, double fov, double near, double far) {
    this->eye = eye;
    this->center = center;
    this->up = up;
    this->fov = fov;
    this->near = near;
    this->far = far;
};
