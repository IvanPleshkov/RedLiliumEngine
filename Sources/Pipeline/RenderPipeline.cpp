#include "pch.h"
#include "RenderPipeline.h"
#include "RenderPass.h"
#include <Render/RenderTarget.h>
#include "StandardPasses/OpaquePass.h"
#include "Components/CameraComponent.h"
#include "Components/MeshRenderer.h"
#include "Components/MeshFilter.h"

using namespace RED_LILIUM_NAMESPACE;

RenderPipeline::RenderPipeline(ptr<RenderDevice> renderDevice)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_targetSize(0.0f, 0.0f)
{}

void RenderPipeline::SetTargetSize(vec2 targetSize)
{
	if (m_targetSize != targetSize)
	{
		m_targetSize = targetSize;
		m_renderTarget = nullptr;
	}
}

bool RenderPipeline::Render(const std::vector<ptr<const Entity>>& roots)
{
	RED_LILIUM_ASSERT(m_targetSize.x > 0.0f && m_targetSize.y > 0.0f && "Pipeline must have target size");

	if (!m_renderTarget)
	{
		m_renderTarget = smake<RenderTarget>(m_renderDevice);
		m_renderTarget->Create(static_cast<u32>(m_targetSize.x), static_cast<u32>(m_targetSize.y));
	}

	m_rootEntities = roots;
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
	m_cameraComponents.clear();
	for (auto rootEntity : m_rootEntities)
	{
		rootEntity->IterateComponentsWithChildren([this](ptr<const Component> component)
		{
			auto cameraComponent = Cast<const CameraComponent>(component);
			if (cameraComponent)
			{
				m_cameraComponents.push_back(cameraComponent);
			}
		});
	}
}

void RenderPipeline::FindRenderables()
{
	m_meshRenderers.clear();
	for (auto rootEntity : m_rootEntities)
	{
		rootEntity->IterateComponentsWithChildren([this](ptr<const Component> component)
		{
			ptr<const MeshRenderer> meshRendererComponent = Cast<const MeshRenderer>(component);
			ptr<const MeshFilter> meshFilter = component->GetEntity()->GetComponent<MeshFilter>();
			if (meshRendererComponent && meshFilter)
			{
				m_meshRenderers.push_back({ meshFilter, meshRendererComponent });
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
