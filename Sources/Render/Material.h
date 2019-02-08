#pragma once

#include "RenderCommon.h"
#include "Uniform.h"

namespace RED_LILIUM_NAMESPACE
{

class Material : public RedLiliumObject
{
public:
	Material(ptr<RenderDevice> renderDevice, std::string_view filename);
	~Material() override;

	template<class T>
	bool Set(std::string_view name, T& value);

/*internal*/ public:
	void Use(ptr<RenderContext> context);
	void SetShaderProgram(const sptr<ShaderProgram>& shaderProgram);
	ptr<VertexDeclaration> GetVertexDeclaration();
	const sptr<ShaderProgram>& GetShaderProgram();

private:
	std::vector<Uniform> m_uniforms;
	std::map<std::string, size_t, std::less<>> m_nameToUniform;
	ptr<RenderDevice> m_renderDevice;
	std::string m_filename;
	ptr<VertexDeclaration> m_vertexDeclaration;
	sptr<ShaderProgram> m_shaderProgram;
};


template<class T>
inline bool Material::Set(std::string_view name, T & value)
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
