#pragma once

#include <Core/Common.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Render/RenderDevice.h>

namespace RED_LILIUM_NAMESPACE
{

class CameraComponent;
class MeshRenderer;

class RenderPipeline : public RedLiliumObject
{
public:
	RenderPipeline(ptr<RenderDevice> renderDevice);
	~RenderPipeline() override = default;

	bool Render(const std::vector<ptr<const Entity>>& roots);

private:
	void FindCameraComponents();
	void FindRenderables();

	std::vector<ptr<const Entity>> m_rootEntities;

	std::vector<ptr<const CameraComponent>> m_cameraComponents;
	std::vector<ptr<const MeshRenderer>> m_meshRenderers;

	ptr<RenderDevice> m_renderDevice;
};

} // namespace RED_LILIUM_NAMESPACE
