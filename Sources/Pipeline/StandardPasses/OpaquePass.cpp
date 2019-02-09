#include "pch.h"
#include "OpaquePass.h"
#include <Render/RenderDevice.h>
#include <Render/RenderContext.h>
#include <Render/Material.h>
#include "../RenderPipeline.h"
#include "../Components/CameraComponent.h"
#include "../Components/MeshRenderer.h"
#include "../Components/MeshFilter.h"

using namespace RED_LILIUM_NAMESPACE;

OpaquePass::OpaquePass(ptr<RenderPipeline> pipeline)
	: RenderPass(pipeline)
{}

bool OpaquePass::Render()
{
	RED_LILIUM_GUARD();

	uptr<RenderContext> renderContext = GetRenderDevice()->CreateRenderContext();

	for (auto& camera : GetCameraComponents())
	{
		Camera cam = camera->GetCamera();
		cam.SetView(camera->GetEntity()->GetWorldTransform() * cam.GetView());

		renderContext->CurrentCamera(cam);
		RenderCamera(renderContext.get(), camera);
	}

	return true;
}

void OpaquePass::RenderCamera(ptr<RenderContext> renderContext, ptr<const CameraComponent> camera)
{
	RED_LILIUM_GUARD();

	for (auto[filter, renderer] : GetMeshRenderers())
	{
		const sptr<Material>& material = renderer->GetMaterial();
		const sptr<GpuMesh>& gpuMesh = filter->GetGpuMesh();
		if (!material || !gpuMesh)
		{
			continue;
		}

		// material->Set("g_model", filter->GetEntity()->GetWorldTransform());
		renderContext->Set("g_model", filter->GetEntity()->GetWorldTransform());
		renderContext->Draw(gpuMesh, material);
	}
}
