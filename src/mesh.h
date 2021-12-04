#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
public:
    std::vector<float> _vertices;
    std::vector<uint16_t> _indices;
};
