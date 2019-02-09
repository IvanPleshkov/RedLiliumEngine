#pragma once

#include "PipelineCommon.h"

namespace RED_LILIUM_NAMESPACE
{

class RenderPass : public RedLiliumObject
{
public:
	RenderPass(ptr<RenderPipeline> pipeline);
	~RenderPass() override = default;

	virtual bool Render() = 0;
	ptr<RenderDevice> GetRenderDevice();

	const std::vector<ptr<const CameraComponent>>& GetCameraComponents() const;
	const std::vector<RenderComponentsPair>& GetMeshRenderers() const;

protected:
	ptr<RenderPipeline> m_pipeline;
};

} // namespace RED_LILIUM_NAMESPACE
