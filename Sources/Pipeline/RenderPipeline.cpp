#include "pch.h"
#include "RenderPipeline.h"
#include "Components/CameraComponent.h"
#include "Components/MeshRenderer.h"

using namespace RED_LILIUM_NAMESPACE;

RenderPipeline::RenderPipeline(ptr<RenderDevice> renderDevice)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
{}

bool RenderPipeline::Render(const std::vector<ptr<const Entity>>& roots)
{
	m_rootEntities = roots;
	FindCameraComponents();
	FindRenderables();

	return true;
}

void RenderPipeline::FindCameraComponents()
{
	m_cameraComponents.clear();
	for (auto rootEntity : m_rootEntities)
	{
		rootEntity->IterateComponentsWithChildren([this](ptr<const Component> component)
		{
			auto cameraComponent = Cast<const CameraComponent>(component);
			m_cameraComponents.push_back(cameraComponent);
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
			auto meshRendererComponent = Cast<const MeshRenderer>(component);
			m_meshRenderers.push_back(meshRendererComponent);
		});
	}
}
