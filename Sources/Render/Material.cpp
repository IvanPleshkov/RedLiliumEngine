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

	m_uniforms = shaderProgram->GetUniforms();
	m_nameToUniform.clear();
	for (size_t i = 0; i < m_uniforms.size(); i++)
	{
		auto& uniform = m_uniforms[i];
		m_nameToUniform.insert({ uniform.name, i });
	}
}

void Material::Use()
{
	glUseProgram(GetShaderProgram()->GetNative());

	for (auto& uniform : m_uniforms)
	{

	}
}
