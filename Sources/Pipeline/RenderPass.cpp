#include "pch.h"
#include "RenderPass.h"
#include "RenderPipeline.h"

using namespace RED_LILIUM_NAMESPACE;

RenderPass::RenderPass(ptr<RenderPipeline> pipeline)
	: RedLiliumObject()
	, m_pipeline(pipeline)
{}

const std::vector<ptr<const CameraComponent>>& RenderPass::GetCameraComponents() const
{
	return m_pipeline->m_perRenderData.m_cameraComponents;
}

const std::vector<ptr<const MeshRenderer>>& RenderPass::GetMeshRenderers() const
{
	return m_pipeline->m_perRenderData.m_meshRenderers;
}
