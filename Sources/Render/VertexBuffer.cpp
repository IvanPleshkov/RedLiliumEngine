#include "pch.h"
#include "VertexBuffer.h"

using namespace RED_LILIUM_NAMESPACE;

VertexBuffer::VertexBuffer(ptr<RenderDevice> renderDevice, std::string_view resourceName, GpuBufferUsage usage)
	: GpuBuffer(renderDevice, resourceName, GL_ARRAY_BUFFER, usage)
{
}

VertexBuffer::~VertexBuffer()
{
}
