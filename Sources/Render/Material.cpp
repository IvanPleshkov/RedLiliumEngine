#include "pch.h"
#include "Material.h"
#include "Shader.h"

using namespace RED_LILIUM_NAMESPACE;

Material::Material(ptr<RenderDevice> renderDevice, const std::string& filename)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_filename(filename)
	, m_vertexDeclaration(nullptr)
{}

Material::~Material()
{}

ptr<VertexDeclaration> Material::GetVertexDeclaration()
{
	RED_LILIUM_ASSERT(m_vertexDeclaration != nullptr);
	return m_vertexDeclaration;
}

const sptr<ShaderProgram>& Material::GetShaderProgram()
{
	RED_LILIUM_ASSERT(m_shaderProgram != nullptr);
	return m_shaderProgram;
}

void Material::SetShaderProgram(const sptr<ShaderProgram>& shaderProgram)
{
	m_shaderProgram = shaderProgram;
	m_vertexDeclaration = m_shaderProgram->GetVertexDeclaration();
}
