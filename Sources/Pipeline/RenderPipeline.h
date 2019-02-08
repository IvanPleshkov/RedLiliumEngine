#pragma once

#include <Core/Common.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Render/RenderDevice.h>

namespace RED_LILIUM_NAMESPACE
{

class RenderPass;
class CameraComponent;
class MeshRenderer;

class RenderPipeline : public RedLiliumObject
{
public:
	RenderPipeline(ptr<RenderDevice> renderDevice);
	~RenderPipeline() override = default;

	bool Render(const std::vector<ptr<const Entity>>& roots);

	static uptr<RenderPipeline> CreateSimpleOpaquePipeline(ptr<RenderDevice> renderDevice);

	template<class T>
	ptr<T> AddPass();
	void SetPassesRelation(ptr<RenderPass> pass, ptr<RenderPass> requiredPass);

	friend class RenderPass;
private:
	void FindCameraComponents();
	void FindRenderables();

	struct PerRenderData
	{
		std::vector<ptr<const Entity>> m_rootEntities;
		std::vector<ptr<const CameraComponent>> m_cameraComponents;
		std::vector<ptr<const MeshRenderer>> m_meshRenderers;

		void Clear();
	};
	PerRenderData m_perRenderData;

	ptr<RenderDevice> m_renderDevice;
	std::vector<uptr<RenderPass>> m_passes;
	std::unordered_map<ptr<RenderPass>, std::vector<ptr<RenderPass>>> m_passRelations;
};

template<class T>
inline ptr<T> RenderPipeline::AddPass()
{
	uptr<T> pass = umake<T>(m_renderDevice);
	ptr<T> result = pass.get();
	m_passes.push_back(std::move(pass));
	return result;
}

} // namespace RED_LILIUM_NAMESPACE
