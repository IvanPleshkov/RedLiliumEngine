#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

class VertexBuffer : public GpuBuffer
{
public:
	VertexBuffer(ptr<RenderDevice> renderDevice, GpuBufferUsage usage = GpuBufferUsage::Static);
	~VertexBuffer() override;
};

} // namespace RED_LILIUM_NAMESPACE
