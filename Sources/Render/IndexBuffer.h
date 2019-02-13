#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

class IndexBuffer : public GpuBuffer
{
public:
	IndexBuffer(ptr<RenderDevice> renderDevice, std::string_view resourceName = "", GpuBufferUsage usage = GpuBufferUsage::Static);
	~IndexBuffer() override;
};

} // namespace RED_LILIUM_NAMESPACE
