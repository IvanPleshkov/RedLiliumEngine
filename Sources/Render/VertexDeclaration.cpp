#include "pch.h"
#include "VertexDeclaration.h"

using namespace RED_LILIUM_NAMESPACE;

VertexDeclaration::VertexDeclaration(ptr<RenderDevice> renderDevice, const std::vector<VertexAttribute>& vertexElements)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_vertexElements(vertexElements)
{}

VertexDeclaration::~VertexDeclaration()
{}

const std::vector<VertexAttribute>& VertexDeclaration::GetVertexElements() const
{
	return m_vertexElements;
}

VertexArrayObject::VertexArrayObject(ptr<RenderDevice> renderDevice, ptr<VertexDeclaration> vertexDeclaration)
	: GpuResource(renderDevice)
{
	glGenVertexArrays(1, &m_handler);
}

VertexArrayObject::~VertexArrayObject()
{}
