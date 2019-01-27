#include "pch.h"
#include "IndexBuffer.h"

using namespace RED_LILIUM_NAMESPACE;

IndexBuffer::IndexBuffer(ptr<RenderDevice> renderDevice, GpuBufferUsage usage)
	: GpuBuffer(renderDevice, GL_ELEMENT_ARRAY_BUFFER, usage)
{}

IndexBuffer::~IndexBuffer()
{}
