#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class Mesh : public RedLiliumObject
{
public:
	~Mesh() override = default;

	static uptr<Mesh> GenerateCube();
	static uptr<Mesh> GenerateTriangle();
	static uptr<Mesh> GenerateRectangle();

	std::vector<u32> m_indices;
	std::vector<vec3> m_positions;
	std::vector<vec3> m_normals;
	std::vector<vec3> m_tangents;
	std::vector<vec3> m_bitangents;
	std::vector<vec4> m_colors0;
	std::vector<vec4> m_colors1;
	std::vector<vec4> m_colors2;
	std::vector<vec4> m_colors3;
	std::vector<vec2> m_texCoords0;
	std::vector<vec2> m_texCoords1;
	std::vector<vec2> m_texCoords2;
	std::vector<vec2> m_texCoords3;
	std::vector<vec2> m_texCoords4;
	std::vector<vec2> m_texCoords5;
	std::vector<vec2> m_texCoords6;
	std::vector<vec2> m_texCoords7;
};

} // namespace RED_LILIUM_NAMESPACE
