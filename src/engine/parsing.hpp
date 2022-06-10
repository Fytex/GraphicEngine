#ifndef PARSING_H
#define PARSING_H

#include <vector>
#include <string>

#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

bool parse(char * filename, Camera &camera, std::vector<Light>& lights, Group& group);

#endif
