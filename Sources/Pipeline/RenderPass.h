#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class CameraComponent;
class MeshRenderer;
class RenderPipeline;

class RenderPass : public RedLiliumObject
{
public:
	RenderPass(ptr<RenderPipeline> pipeline);
	~RenderPass() override = default;

	virtual bool Render() = 0;

	const std::vector<ptr<const CameraComponent>>& GetCameraComponents() const;
	const std::vector<ptr<const MeshRenderer>>& GetMeshRenderers() const;

protected:
	ptr<RenderPipeline> m_pipeline;
};

} // namespace RED_LILIUM_NAMESPACE
