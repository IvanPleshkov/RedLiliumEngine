#pragma once

#include "RenderCommon.h"
#include "Uniform.h"

namespace RED_LILIUM_NAMESPACE
{

class Material : public RedLiliumObject
{
public:
	Material(ptr<RenderDevice> renderDevice, const std::string& filename);
	~Material() override;
	ptr<VertexDeclaration> GetVertexDeclaration();
	const sptr<ShaderProgram>& GetShaderProgram();

	void Use();
	void SetShaderProgram(const sptr<ShaderProgram>& shaderProgram);
	
	// todo: use string_view because most of uniform names in code are const char*
	template<class T>
	bool Set(const std::string& name, T& value);

private:
	std::vector<Uniform> m_uniforms;
	std::map<std::string, size_t> m_nameToUniform;
	ptr<RenderDevice> m_renderDevice;
	std::string m_filename;
	ptr<VertexDeclaration> m_vertexDeclaration;
	sptr<ShaderProgram> m_shaderProgram;
};


template<class T>
inline bool Material::Set(const std::string & name, T & value)
{
	auto i = m_nameToUniform.find(name);
	if (i == m_nameToUniform.end())
	{
		return false;
	}
	m_uniforms[i->second].Set(value);
	return true;
}

} // namespace RED_LILIUM_NAMESPACE
