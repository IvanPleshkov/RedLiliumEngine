#pragma once

#include "RenderCommon.h"

namespace RED_LILIUM_NAMESPACE
{

class RenderContext : public RedLiliumObject
{
public:
	RenderContext(ptr<RenderDevice> renderDevice);
	~RenderContext();

	void Draw(const sptr<GpuMesh>& mesh, const sptr<Material>& material);

private:
	ptr<RenderDevice> m_renderDevice;
};

} // namespace RED_LILIUM_NAMESPACE
