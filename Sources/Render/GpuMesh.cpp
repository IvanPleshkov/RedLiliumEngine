#include "pch.h"
#include "GpuMesh.h"
#include "Material.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "RenderDevice.h"

using namespace RED_LILIUM_NAMESPACE;

namespace
{
template<class T>
void InitVertexBuffer(
	uptr<VertexBuffer>& vertexBuffer,
	const std::vector<T>& data,
	ptr<RenderDevice> renderDevice,
	std::string_view resourceName,
	GpuBufferUsage usage)
{
	vertexBuffer = nullptr; // Render TODO: reuse buffer, dont't delete
	if (!data.empty())
	{
		vertexBuffer = umake<VertexBuffer>(renderDevice, resourceName, usage);
		vertexBuffer->SendData(data.data(), data.size() * sizeof(T));
	}
}
}

GpuMesh::GpuMesh(ptr<RenderDevice> renderDevice, std::string_view resourceName, GpuBufferUsage usage)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_resourceName(resourceName)
	, m_usage(usage)
	, m_arrayObjects()
	, m_indicesSize(0)
	, m_indices(nullptr)
	, m_positions(nullptr)
	, m_normals(nullptr)
	, m_tangents(nullptr)
	, m_bitangents(nullptr)
{
	for (auto& color : m_colors)
	{
		color = nullptr;
	}

	for (auto& texCoord : m_texCoords)
	{
		texCoord = nullptr;
	}
}

GpuMesh::~GpuMesh()
{}

void GpuMesh::Update(ptr<const Mesh> mesh, IndicesBufferFormat indexBufferFormat)
{
	// TODO: check if changes type of data
	m_arrayObjects.clear();

	m_indicesSize = mesh->m_indices.size();
	m_indices = nullptr;
	RED_LILIUM_ASSERT(!mesh->m_indices.empty());
	m_indices = umake<IndexBuffer>(m_renderDevice, m_resourceName + "_indices", m_usage);
	m_indices->SendData(mesh->m_indices.data(), mesh->m_indices.size() * sizeof(u32));

	InitVertexBuffer(m_positions, mesh->m_positions, m_renderDevice, m_resourceName + "_positions", m_usage);
	InitVertexBuffer(m_normals, mesh->m_normals, m_renderDevice, m_resourceName + "_normals", m_usage);
	InitVertexBuffer(m_tangents, mesh->m_tangents, m_renderDevice, m_resourceName + "_tangents", m_usage);
	InitVertexBuffer(m_bitangents, mesh->m_bitangents, m_renderDevice, m_resourceName + "_bitangents", m_usage);

	for (u32 i = 0; i < Mesh::COLORS_COUNT; i++)
	{
		InitVertexBuffer(m_colors[i], mesh->m_colors[i], m_renderDevice, m_resourceName + "_colors", m_usage);
	}

	for (u32 i = 0; i < Mesh::TEXCOORDS_COUNT; i++)
	{
		InitVertexBuffer(m_texCoords[i], mesh->m_texCoords[i], m_renderDevice, m_resourceName + "_texcoords", m_usage);
	}
}

void GpuMesh::UpdateIndices(ptr<const Mesh> mesh, IndicesBufferFormat indexBufferFormat)
{

}

u32 GpuMesh::GetIndicesSize() const
{
	return m_indicesSize;
}

ptr<VertexArrayObject> GpuMesh::GetVertexArrayObject(ptr<VertexDeclaration> vertexDeclaration)
{
	auto i = m_arrayObjects.find(vertexDeclaration);
	if (i == m_arrayObjects.end())
	{
		uptr<VertexArrayObject> arrayObject = umake<VertexArrayObject>(m_renderDevice, vertexDeclaration);
		ptr<VertexArrayObject> result = arrayObject.get();
		InitVertexArrayObject(result, vertexDeclaration);
		m_arrayObjects.insert({ vertexDeclaration, std::move(arrayObject) });
		return result;
	}
	else
	{
		return i->second.get();
	}
}

void GpuMesh::InitVertexArrayObject(ptr<VertexArrayObject> va, ptr<VertexDeclaration> vdecl)
{
	glBindVertexArray(va->GetNative());
	m_indices->Bind();

	auto& elements = vdecl->GetVertexElements();
	for (auto& e : elements)
	{
		glEnableVertexAttribArray(e.layout);
	}

	for (auto& e : elements)
	{
		switch (e.vertexAttribute)
		{
		case VertexAttribute::Position:
			m_positions->Bind();
			glVertexAttribPointer(e.layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Normal:
			m_normals->Bind();
			glVertexAttribPointer(e.layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Tangent:
			m_tangents->Bind();
			glVertexAttribPointer(e.layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Bitangent:
			m_bitangents->Bind();
			glVertexAttribPointer(e.layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color0:
			m_colors[0]->Bind();
			glVertexAttribPointer(e.layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color1:
			m_colors[1]->Bind();
			glVertexAttribPointer(e.layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color2:
			m_colors[2]->Bind();
			glVertexAttribPointer(e.layout, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color3:
			m_colors[3]->Bind();
			glVertexAttribPointer(e.layout, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord0:
			m_texCoords[0]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord1:
			m_texCoords[1]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord2:
			m_texCoords[2]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord3:
			m_texCoords[3]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord4:
			m_texCoords[4]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord5:
			m_texCoords[5]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord6:
			m_texCoords[6]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord7:
			m_texCoords[7]->Bind();
			glVertexAttribPointer(e.layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		}
	}

	glBindVertexArray(0);
}
