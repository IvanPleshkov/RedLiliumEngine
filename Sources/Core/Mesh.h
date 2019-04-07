#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class Mesh : public RedLiliumObject
{
public:
	~Mesh() override = default;

	static const u32 TEXCOORDS_COUNT = 8;
	static const u32 COLORS_COUNT = 4;

	static sptr<Mesh> GenerateCube();
	static sptr<Mesh> GenerateTriangle();
	static sptr<Mesh> GenerateRectangle();

	std::vector<u32> m_indices;
	std::vector<vec3> m_positions;
	std::vector<vec3> m_normals;
	std::vector<vec3> m_tangents;
	std::vector<vec3> m_bitangents;

	std::array<std::vector<vec4>, COLORS_COUNT> m_colors;
	std::array<std::vector<vec2>, TEXCOORDS_COUNT> m_texCoords;
};

} // namespace RED_LILIUM_NAMESPACE
