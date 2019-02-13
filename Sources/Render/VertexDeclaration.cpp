#include "pch.h"
#include "VertexDeclaration.h"

using namespace RED_LILIUM_NAMESPACE;

VertexDeclaration::VertexDeclaration(ptr<RenderDevice> renderDevice, const std::vector<VertexInput>& verts)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_vertexElements(verts)
{}

VertexDeclaration::~VertexDeclaration()
{}

const std::vector<VertexInput>& VertexDeclaration::GetVertexElements() const
{
	return m_vertexElements;
}

VertexArrayObject::VertexArrayObject(ptr<RenderDevice> renderDevice, ptr<VertexDeclaration> vertexDeclaration)
	: GpuResource(renderDevice, "vao")
{
	glGenVertexArrays(1, &m_handler);
}

VertexArrayObject::~VertexArrayObject()
{
	if (m_handler != 0)
	{
		glDeleteVertexArrays(1, &m_handler);
		m_handler = 0;
	}
}
