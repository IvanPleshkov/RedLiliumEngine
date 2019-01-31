#include "pch.h"
#include "GpuBuffer.h"

using namespace RED_LILIUM_NAMESPACE;

GpuBuffer::GpuBuffer(ptr<RenderDevice> renderDevice, GLenum bufferType, GpuBufferUsage usage)
	: GpuResource(renderDevice)
	, m_bufferType(bufferType)
	, m_usage(usage)
{
	glGenBuffers(1, &m_handler);
}

GpuBuffer::~GpuBuffer()
{
	if (m_handler != 0)
	{
		glDeleteBuffers(1, &m_handler);
		m_handler = 0;
	}
}

bool GpuBuffer::SendData(const void* data, u64 size)
{
	Bind();
	glBufferData(m_bufferType, static_cast<GLsizeiptr>(size), data, GetGLUsage());
	Undind();
	return true;
}

void GpuBuffer::Bind()
{
	glBindBuffer(m_bufferType, m_handler);
}

void GpuBuffer::Undind()
{
	glBindBuffer(m_bufferType, 0);
}

GLenum GpuBuffer::GetGLUsage() const
{
	switch (m_usage)
	{
	case GpuBufferUsage::Static:
		return GL_STATIC_DRAW;
	case GpuBufferUsage::Dynamic:
		return GL_DYNAMIC_DRAW;
	case GpuBufferUsage::Stream:
		return GL_STREAM_DRAW;
	default:
		return GL_STATIC_DRAW;
	}
}
