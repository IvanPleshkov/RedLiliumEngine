#pragma once

#include "RenderCommon.h"
#include "VertexBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

enum class IndicesBufferFormat
{
	Auto = 1 << 0,
	Uint16 = 1 << 1,
	Uint32 = 1 << 2,
};

class GpuMesh : public RedLiliumObject
{
public:
	GpuMesh(ptr<RenderDevice> renderDevice, std::string_view resourceName = "", GpuBufferUsage usage = GpuBufferUsage::Static);
	~GpuMesh() override;

	void Update(ptr<const Mesh> mesh, IndicesBufferFormat indexBufferFormat = IndicesBufferFormat::Auto);
	const std::string& GetName() const { return m_resourceName; }

RED_LILIUM_INTERNAL:
	ptr<VertexArrayObject> GetVertexArrayObject(ptr<VertexDeclaration> vertexDeclaration);
	u32 GetIndicesSize() const;

private:
	void UpdateIndices(ptr<const Mesh> mesh, IndicesBufferFormat indexBufferFormat);
	void InitVertexArrayObject(ptr<VertexArrayObject> va, ptr<VertexDeclaration> vdecl);

	ptr<RenderDevice> m_renderDevice;
	std::string m_resourceName;
	GpuBufferUsage m_usage;
	std::unordered_map<ptr<VertexDeclaration>, uptr<VertexArrayObject>> m_arrayObjects;

	u32 m_indicesSize;
	uptr<IndexBuffer> m_indices;
	IndicesBufferFormat m_indicesBufferFormat;

	uptr<VertexBuffer> m_positions;
	uptr<VertexBuffer> m_normals;
	uptr<VertexBuffer> m_tangents;
	uptr<VertexBuffer> m_bitangents;
	
	std::array<uptr<VertexBuffer>, Mesh::COLORS_COUNT> m_colors;
	std::array<uptr<VertexBuffer>, Mesh::TEXCOORDS_COUNT> m_texCoords;
};

} // namespace RED_LILIUM_NAMESPACE
