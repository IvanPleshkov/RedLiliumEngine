#include "pch.h"
#include "RenderContext.h"
#include "Material.h"
#include "Shader.h"
#include "GpuMesh.h"
#include "VertexDeclaration.h"
#include "RenderDevice.h"
#include "RenderTarget.h"
#include "Uniform.h"

using namespace RED_LILIUM_NAMESPACE;

RenderContext::RenderContext(ptr<RenderDevice> renderDevice)
	: m_renderDevice(renderDevice)
{
}

RenderContext::~RenderContext()
{
	SetRenderTarget(nullptr);
}

void RenderContext::Draw(const sptr<GpuMesh>& mesh, const sptr<Material>& material)
{
	UpdateUniformBlocks();
	material->Use(this);

	const sptr<ShaderProgram>& shaderProgram = material->GetShaderProgram();
	ptr<VertexArrayObject> vao = mesh->GetVertexArrayObject(shaderProgram->GetVertexDeclaration());
	glBindVertexArray(vao->GetNative());
	glDrawElements(GL_TRIANGLES, mesh->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderContext::CurrentCamera(const Camera& camera)
{
	m_currentCamera = camera;
	Set("g_view", camera.GetView());
	Set("g_proj", camera.GetProj());
	mat4 viewProj = camera.GetProj() * camera.GetView();
	Set("g_viewProj", viewProj);
}

void RenderContext::SetRenderTarget(const sptr<RenderTarget>& renderTarget)
{
	m_currentRenderTarget = renderTarget;
	if (m_currentRenderTarget)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->GetNative());
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

const sptr<RenderTarget>& RenderContext::SetRenderTarget()
{
	return m_currentRenderTarget;
}

void RenderContext::UpdateUniformBlocks()
{
	std::unordered_set<ptr<UniformBlock>> changedBlocks;
	for (auto&[ uniform, block ] : m_uniformBlocksToUpdate)
	{
		uniform->SendToBlock(block);
		changedBlocks.insert(block);
	}
	for (auto& block : changedBlocks)
	{
		block->SendData();
	}
}

ptr<const Uniform::ValueVariants> RenderContext::GetUniformValue(std::string_view name)
{
	auto i = m_contextUniforms.find(name);
	if (i != m_contextUniforms.end())
	{
		return &i->second;
	}

	auto u = m_renderDevice->GetGlobalUniform(name);
	if (u.has_value())
	{
		return &u.value().first->GetValue();
	}

	return nullptr;
}

void RenderContext::Clear()
{
	m_contextUniforms.clear();
}
