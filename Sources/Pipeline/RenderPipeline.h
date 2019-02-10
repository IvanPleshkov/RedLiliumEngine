#pragma once

#include "PipelineCommon.h"

namespace RED_LILIUM_NAMESPACE
{

class RenderPipeline : public RedLiliumObject
{
public:
	RenderPipeline(ptr<RenderDevice> renderDevice);
	~RenderPipeline() override = default;

	void SetTargetSize(vec2 targetSize);
	bool Render(const std::vector<ptr<const Entity>>& roots);

	template<class T>
	ptr<T> AddPass();
	void SetPassesRelation(ptr<RenderPass> pass, ptr<RenderPass> requiredPass);

	const sptr<RenderTarget>& GetRenderTarget() const { return m_renderTarget; }

	static uptr<RenderPipeline> CreateSimpleOpaquePipeline(ptr<RenderDevice> renderDevice);

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
	vec2 m_targetSize;
	sptr<RenderTarget> m_renderTarget;
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
