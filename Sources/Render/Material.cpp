#include "pch.h"
#include "Material.h"

using namespace RED_LILIUM_NAMESPACE;

Material::Material()
	: m_vertexDeclaration(nullptr)
{}

Material::~Material()
{}

ptr<VertexDeclaration> Material::GetVertexDeclaration()
{
	RED_LILIUM_ASSERT(m_vertexDeclaration != nullptr);
	return m_vertexDeclaration;
}

void Material::SetVertexDeclaration(ptr<VertexDeclaration> vertexDeclaration)
{
	m_vertexDeclaration = vertexDeclaration;
}

const sptr<ShaderProgram>& Material::GetShaderProgram()
{
	RED_LILIUM_ASSERT(m_shaderProgram != nullptr);
	return m_shaderProgram;
}

void Material::SetShaderProgram(const sptr<ShaderProgram>& shaderProgram)
{
	m_shaderProgram = shaderProgram;
}
