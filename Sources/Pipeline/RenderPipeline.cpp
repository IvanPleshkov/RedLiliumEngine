#include "pch.h"
#include "RenderPipeline.h"
#include "RenderPass.h"
#include "StandardPasses/OpaquePass.h"
#include "Components/CameraComponent.h"
#include "Components/MeshRenderer.h"

using namespace RED_LILIUM_NAMESPACE;

RenderPipeline::RenderPipeline(ptr<RenderDevice> renderDevice)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_perRenderData()
{}

bool RenderPipeline::Render(const std::vector<ptr<const Entity>>& roots)
{
	m_perRenderData.Clear();
	m_perRenderData.m_rootEntities = roots;
	FindCameraComponents();
	FindRenderables();

	bool res = true;
	for (auto& pass : m_passes)
	{
		res &= pass->Render();
	}

	return res;
}

void RenderPipeline::FindCameraComponents()
{
	for (auto rootEntity : m_perRenderData.m_rootEntities)
	{
		rootEntity->IterateComponentsWithChildren([this](ptr<const Component> component)
		{
			auto cameraComponent = Cast<const CameraComponent>(component);
			if (cameraComponent)
			{
				m_perRenderData.m_cameraComponents.push_back(cameraComponent);
			}
		});
	}
}

void RenderPipeline::FindRenderables()
{
	for (auto rootEntity : m_perRenderData.m_rootEntities)
	{
		rootEntity->IterateComponentsWithChildren([this](ptr<const Component> component)
		{
			auto meshRendererComponent = Cast<const MeshRenderer>(component);
			if (meshRendererComponent)
			{
				m_perRenderData.m_meshRenderers.push_back(meshRendererComponent);
			}
		});
	}
}

uptr<RenderPipeline> RenderPipeline::CreateSimpleOpaquePipeline(ptr<RenderDevice> renderDevice)
{
	uptr<RenderPipeline> pipeline = umake<RenderPipeline>(renderDevice);
	pipeline->AddPass<OpaquePass>();
	return std::move(pipeline);
}

void RenderPipeline::SetPassesRelation(ptr<RenderPass> pass, ptr<RenderPass> requiredPass)
{
	RED_LILIUM_NOT_IMPLEMENTED();
}

void RenderPipeline::PerRenderData::Clear()
{
	m_rootEntities.clear();
	m_cameraComponents.clear();
	m_meshRenderers.clear();
}
