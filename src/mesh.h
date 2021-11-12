#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
};

class Mesh
{
public:
    std::vector<Vertex> _vertices;
    std::vector<uint16_t> _indices;
};
