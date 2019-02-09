#include "pch.h"
#include "RenderPass.h"
#include "RenderPipeline.h"

using namespace RED_LILIUM_NAMESPACE;

RenderPass::RenderPass(ptr<RenderPipeline> pipeline)
	: RedLiliumObject()
	, m_pipeline(pipeline)
{}

ptr<RenderDevice> RenderPass::GetRenderDevice()
{
	return m_pipeline->m_renderDevice;
}

const std::vector<ptr<const CameraComponent>>& RenderPass::GetCameraComponents() const
{
	return m_pipeline->m_cameraComponents;
}

const std::vector<RenderComponentsPair>& RenderPass::GetMeshRenderers() const
{
	return m_pipeline->m_meshRenderers;
}
