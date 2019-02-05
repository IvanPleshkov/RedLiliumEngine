#include "pch.h"
#include "RenderContext.h"
#include "Material.h"
#include "Shader.h"
#include "GpuMesh.h"
#include "VertexDeclaration.h"
#include "RenderDevice.h"
#include "Uniform.h"

using namespace RED_LILIUM_NAMESPACE;

RenderContext::RenderContext(ptr<RenderDevice> renderDevice)
	: m_renderDevice(renderDevice)
{
}

RenderContext::~RenderContext()
{}

void RenderContext::Draw(const sptr<GpuMesh>& mesh, const sptr<Material>& material)
{
	UpdateUniformBlocks();
	material->Use(this);

	ptr<VertexArrayObject> vao = mesh->GetVertexArrayObject(material->GetVertexDeclaration());
	glBindVertexArray(vao->GetNative());
	glDrawElements(GL_TRIANGLES, mesh->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderContext::CurrentCamera(ptr<Camera> camera)
{
	m_currentCamera = camera;
	Set("g_view", camera->GetView());
	Set("g_proj", camera->GetProj());
	mat4 viewProj = camera->GetProj() * camera->GetView();
	Set("g_viewProj", viewProj);
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
