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
	renderContext->SetRenderTarget(m_pipeline->GetRenderTarget());

	glViewport(0, 0, static_cast<GLsizei>(m_pipeline->GetTargetSize().x), static_cast<GLsizei>(m_pipeline->GetTargetSize().y));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		mat4 worldTransform = filter->GetEntity()->GetWorldTransform();
		renderContext->Set("g_model", worldTransform);
		renderContext->Draw(gpuMesh, material);
	}
}
