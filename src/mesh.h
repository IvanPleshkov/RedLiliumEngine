#pragma once

#include <glm/glm.hpp>
#include <vector>

class Mesh
{
public:
    std::vector<float> _vertices;
    std::vector<uint16_t> _indices;
};
