#include "pch.h"
#include "VertexBuffer.h"

using namespace RED_LILIUM_NAMESPACE;

VertexBuffer::VertexBuffer(ptr<RenderDevice> renderDevice, GpuBufferUsage usage)
	: GpuBuffer(renderDevice, GL_ARRAY_BUFFER, usage)
{
}

VertexBuffer::~VertexBuffer()
{
}
