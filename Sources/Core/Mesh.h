#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

enum class MeshPrimitiveType : u8
{
	PointList			= 1 << 0,
	LineList			= 1 << 1,
	LineStrip			= 1 << 2,
	TriangleList		= 1 << 3,
	TriangleStrip		= 1 << 4,
	TriangleFan			= 1 << 5,
};

class Mesh : public RedLiliumObject
{
public:
	~Mesh() override = default;

	static const u32 TEXCOORDS_COUNT = 8;
	static const u32 COLORS_COUNT = 4;

	static sptr<Mesh> GenerateCube();
	static sptr<Mesh> GenerateTriangle();
	static sptr<Mesh> GenerateRectangle();

	void Save(std::ostream& stream);
	void Save(FileSystem& filesystem, std::string_view filename);
	void Load(std::istream& stream);
	void Load(FileSystem& filesystem, std::string_view filename);

	MeshPrimitiveType m_primitiveType;

	std::vector<u32> m_indices;
	std::vector<vec3> m_positions;
	std::vector<vec3> m_normals;
	std::vector<vec3> m_tangents;
	std::vector<vec3> m_bitangents;

	std::array<std::vector<vec4>, COLORS_COUNT> m_colors;
	std::array<std::vector<vec2>, TEXCOORDS_COUNT> m_texCoords;
};

} // namespace RED_LILIUM_NAMESPACE
