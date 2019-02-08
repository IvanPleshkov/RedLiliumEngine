#include "pch.h"
#include "OpaquePass.h"
#include "../RenderPipeline.h"
#include "../Components/CameraComponent.h"

using namespace RED_LILIUM_NAMESPACE;

OpaquePass::OpaquePass(ptr<RenderPipeline> pipeline)
	: RenderPass(pipeline)
{}

bool OpaquePass::Render()
{
	for (auto& camera : GetCameraComponents())
	{
		RenderCamera(camera);
	}

	return true;
}

void OpaquePass::RenderCamera(ptr<const CameraComponent> camera)
{
	for (auto& renderer : GetMeshRenderers())
	{

	}
}
