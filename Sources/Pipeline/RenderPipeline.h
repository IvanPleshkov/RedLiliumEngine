#pragma once

#include "PipelineCommon.h"

namespace RED_LILIUM_NAMESPACE
{

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

	ptr<RenderDevice> m_renderDevice;
	std::vector<uptr<RenderPass>> m_passes;
	std::unordered_map<ptr<RenderPass>, std::vector<ptr<RenderPass>>> m_passRelations;

// per render data
private:
	std::vector<ptr<const Entity>> m_rootEntities;
	std::vector<ptr<const CameraComponent>> m_cameraComponents;
	std::vector<RenderComponentsPair> m_meshRenderers;
};

template<class T>
inline ptr<T> RenderPipeline::AddPass()
{
	uptr<T> pass = umake<T>(this);
	ptr<T> result = pass.get();
	m_passes.push_back(std::move(pass));
	return result;
}

} // namespace RED_LILIUM_NAMESPACE
