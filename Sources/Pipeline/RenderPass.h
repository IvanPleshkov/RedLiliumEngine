#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class RenderPipeline;

class RenderPass : public RedLiliumObject
{
public:
	RenderPass(ptr<RenderPipeline> pipeline);
	~RenderPass() override = default;

	virtual bool Render() = 0;

protected:
	ptr<RenderPipeline> m_pipeline;
};

} // namespace RED_LILIUM_NAMESPACE
