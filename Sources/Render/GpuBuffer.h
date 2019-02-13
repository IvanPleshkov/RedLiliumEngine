#pragma once

#include "RenderCommon.h"

namespace RED_LILIUM_NAMESPACE
{

class GpuResource : public RedLiliumObject
{
public:
	GpuResource(ptr<RenderDevice> renderDevice, std::string_view resourceName)
		: m_renderDevice(renderDevice)
		, m_resourceName(resourceName)
		, m_handler(0)
	{}
	~GpuResource() override = default;

	ptr<RenderDevice> GetRenderDevice() { return m_renderDevice; }
	const std::string& GetName() const { return m_resourceName; }

RED_LILIUM_INTERNAL:
	GLuint GetNative() const { return m_handler; }

protected:
	GLuint m_handler;
	ptr<RenderDevice> m_renderDevice;
	std::string m_resourceName;
};

enum class GpuBufferUsage : u8
{
	Static = 1 << 0, // seldom update
	Dynamic = 1 << 1, // often change
	Stream = 1 << 2, // change each frame
};

class GpuBuffer : public GpuResource
{
public:
	GpuBuffer(ptr<RenderDevice> renderDevice, std::string_view resourceName, GLenum bufferType, GpuBufferUsage usage);
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
