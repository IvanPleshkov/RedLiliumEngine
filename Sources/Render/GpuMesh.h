#pragma once

#include "RenderCommon.h"
#include "VertexBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

class GpuMesh : public RedLiliumObject
{
public:
	GpuMesh(ptr<RenderDevice> renderDevice, GpuBufferUsage usage = GpuBufferUsage::Static);
	~GpuMesh() override;

	void Update(ptr<const Mesh> mesh);
	ptr<VertexArrayObject> GetVertexArrayObject(ptr<VertexDeclaration> vertexDeclaration);
	u32 GetIndicesSize() const;

private:
	void InitVertexArrayObject(ptr<VertexArrayObject> va, ptr<VertexDeclaration> vdecl);

	ptr<RenderDevice> m_renderDevice;
	GpuBufferUsage m_usage;
	std::unordered_map<ptr<VertexDeclaration>, uptr<VertexArrayObject>> m_arrayObjects;

	u32 m_indicesSize;
	uptr<IndexBuffer> m_indices;
	uptr<VertexBuffer> m_positions;
	uptr<VertexBuffer> m_normals;
	uptr<VertexBuffer> m_tangents;
	uptr<VertexBuffer> m_bitangents;
	uptr<VertexBuffer> m_colors0;
	uptr<VertexBuffer> m_colors1;
	uptr<VertexBuffer> m_colors2;
	uptr<VertexBuffer> m_colors3;
	uptr<VertexBuffer> m_texCoords0;
	uptr<VertexBuffer> m_texCoords1;
	uptr<VertexBuffer> m_texCoords2;
	uptr<VertexBuffer> m_texCoords3;
	uptr<VertexBuffer> m_texCoords4;
	uptr<VertexBuffer> m_texCoords5;
	uptr<VertexBuffer> m_texCoords6;
	uptr<VertexBuffer> m_texCoords7;
};

} // namespace RED_LILIUM_NAMESPACE
