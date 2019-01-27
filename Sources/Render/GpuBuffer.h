#pragma once

#include "RenderCommon.h"

namespace RED_LILIUM_NAMESPACE
{

class GpuResource : public RedLiliumObject
{
public:
	GpuResource(ptr<RenderDevice> renderDevice)
		: m_renderDevice(renderDevice)
	{}
	~GpuResource() override = default;

	GLuint GetNative() const { return m_handler; }
	ptr<RenderDevice> GetRenderDevice() { return m_renderDevice; }

protected:
	GLuint m_handler;
	ptr<RenderDevice> m_renderDevice;
};

enum class GpuBufferUsage : u8
{
	Static = 1 << 0, // seldom update
	Dynamic = 1 << 1, // often chanmge
	Stream = 1 << 2, // change each frame
};

class GpuBuffer : public GpuResource
{
public:
	GpuBuffer(ptr<RenderDevice> renderDevice, GLenum bufferType, GpuBufferUsage usage);
	~GpuBuffer() override;

	bool SendData(const void* data, u64 size);

	void Bind();
	void Undind();
	GLenum GetGLUsage() const;

protected:
	GLenum m_bufferType;
	GpuBufferUsage m_usage;
};

} // namespace RED_LILIUM_NAMESPACE
