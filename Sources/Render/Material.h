#pragma once

#include "RenderCommon.h"

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

private:
	std::vector<Uniform> m_uniforms;
	std::map<std::string, size_t> m_nameToUniform;
	ptr<RenderDevice> m_renderDevice;
	std::string m_filename;
	ptr<VertexDeclaration> m_vertexDeclaration;
	sptr<ShaderProgram> m_shaderProgram;
};

} // namespace RED_LILIUM_NAMESPACE
