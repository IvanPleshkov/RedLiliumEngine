#include "pch.h"
#include "IndexBuffer.h"

using namespace RED_LILIUM_NAMESPACE;

IndexBuffer::IndexBuffer(ptr<RenderDevice> renderDevice, std::string_view resourceName, GpuBufferUsage usage)
	: GpuBuffer(renderDevice, resourceName, GL_ELEMENT_ARRAY_BUFFER, usage)
{}

IndexBuffer::~IndexBuffer()
{}
