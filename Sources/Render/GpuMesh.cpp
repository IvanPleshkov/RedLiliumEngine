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
void InitVertexBuffer(uptr<VertexBuffer>& vertexBuffer, const std::vector<T>& data, ptr<RenderDevice> renderDevice, GpuBufferUsage usage)
{
	vertexBuffer = nullptr;
	if (!data.empty())
	{
		vertexBuffer = umake<VertexBuffer>(renderDevice, usage);
		vertexBuffer->SendData(data.data(), data.size() * sizeof(T));
	}
}
}

GpuMesh::GpuMesh(ptr<RenderDevice> renderDevice, GpuBufferUsage usage)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_usage(usage)
	, m_arrayObjects()
	, m_indicesSize(0)
	, m_indices(nullptr)
	, m_positions(nullptr)
	, m_normals(nullptr)
	, m_tangents(nullptr)
	, m_bitangents(nullptr)
	, m_colors0(nullptr)
	, m_colors1(nullptr)
	, m_colors2(nullptr)
	, m_colors3(nullptr)
	, m_texCoords0(nullptr)
	, m_texCoords1(nullptr)
	, m_texCoords2(nullptr)
	, m_texCoords3(nullptr)
	, m_texCoords4(nullptr)
	, m_texCoords5(nullptr)
	, m_texCoords6(nullptr)
	, m_texCoords7(nullptr)
{
	// m_indexBuffer = umake<IndexBuffer>(usage);
	// m_vertexBuffer = umake<VertexBuffer>(material->GetVertexDeclaration(), usage);
}

GpuMesh::~GpuMesh()
{}

void GpuMesh::Update(ptr<const Mesh> mesh)
{
	// TODO: check if changes type of data
	m_arrayObjects.clear();

	m_indicesSize = mesh->m_indices.size();
	m_indices = nullptr;
	RED_LILIUM_ASSERT(!mesh->m_indices.empty());
	m_indices = umake<IndexBuffer>(m_renderDevice, m_usage);
	m_indices->SendData(mesh->m_indices.data(), mesh->m_indices.size() * sizeof(u32));

	InitVertexBuffer(m_positions, mesh->m_positions, m_renderDevice, m_usage);
	InitVertexBuffer(m_normals, mesh->m_normals, m_renderDevice, m_usage);
	InitVertexBuffer(m_tangents, mesh->m_tangents, m_renderDevice, m_usage);
	InitVertexBuffer(m_bitangents, mesh->m_bitangents, m_renderDevice, m_usage);
	InitVertexBuffer(m_colors0, mesh->m_colors0, m_renderDevice, m_usage);
	InitVertexBuffer(m_colors1, mesh->m_colors1, m_renderDevice, m_usage);
	InitVertexBuffer(m_colors2, mesh->m_colors2, m_renderDevice, m_usage);
	InitVertexBuffer(m_colors3, mesh->m_colors3, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords0, mesh->m_texCoords0, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords1, mesh->m_texCoords1, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords2, mesh->m_texCoords2, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords3, mesh->m_texCoords3, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords4, mesh->m_texCoords4, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords5, mesh->m_texCoords5, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords6, mesh->m_texCoords6, m_renderDevice, m_usage);
	InitVertexBuffer(m_texCoords7, mesh->m_texCoords7, m_renderDevice, m_usage);
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
	for (u32 i = 0; i < elements.size(); i++)
	{
		glEnableVertexAttribArray(i);
	}

	for (u32 i = 0; i < elements.size(); i++)
	{
		switch (elements[i])
		{
		case VertexAttribute::Position:
			m_positions->Bind();
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Normal:
			m_normals->Bind();
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Tangent:
			m_tangents->Bind();
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Bitangent:
			m_bitangents->Bind();
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color0:
			m_colors0->Bind();
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color1:
			m_colors1->Bind();
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color2:
			m_colors2->Bind();
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::Color3:
			m_colors3->Bind();
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord0:
			m_texCoords0->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord1:
			m_texCoords1->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord2:
			m_texCoords2->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord3:
			m_texCoords3->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord4:
			m_texCoords4->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord5:
			m_texCoords5->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord6:
			m_texCoords6->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		case VertexAttribute::TexCoord7:
			m_texCoords7->Bind();
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
			break;
		}
	}

	glBindVertexArray(0);
}
