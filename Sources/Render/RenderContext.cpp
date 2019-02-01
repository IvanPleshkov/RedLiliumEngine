#include "pch.h"
#include "RenderContext.h"
#include "Material.h"
#include "Shader.h"
#include "GpuMesh.h"
#include "VertexDeclaration.h"
#include "RenderDevice.h"

using namespace RED_LILIUM_NAMESPACE;

RenderContext::RenderContext(ptr<RenderDevice> renderDevice)
	: m_renderDevice(renderDevice)
{
}

RenderContext::~RenderContext()
{}

void RenderContext::Draw(const sptr<GpuMesh>& mesh, const sptr<Material>& material)
{
	material->Use();

	ptr<VertexArrayObject> vao = mesh->GetVertexArrayObject(material->GetVertexDeclaration());
	glBindVertexArray(vao->GetNative());
	glDrawElements(GL_TRIANGLES, mesh->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_renderDevice->CheckErrors();
}
