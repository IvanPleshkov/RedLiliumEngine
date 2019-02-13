#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

class VertexBuffer : public GpuBuffer
{
public:
	VertexBuffer(ptr<RenderDevice> renderDevice, std::string_view resourceName, GpuBufferUsage usage = GpuBufferUsage::Static);
	~VertexBuffer() override;
};

} // namespace RED_LILIUM_NAMESPACE
