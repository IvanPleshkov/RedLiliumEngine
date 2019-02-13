#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"
#include "Uniform.h"

namespace RED_LILIUM_NAMESPACE
{

class Shader : public GpuResource
{
public:
	Shader(ptr<RenderDevice> renderDevice, std::string_view resourceName);
	~Shader() override;

	void CompileFromString(ShaderType type, const std::string& shader, const std::string& includeFilename = "");

private:
	ShaderType m_type;
};


class ShaderProgram : public GpuResource
{
public:
	ShaderProgram(ptr<RenderDevice> renderDevice, std::string_view resourceName);
	~ShaderProgram() override;

	void Link(const sptr<Shader>& vertexShader, const sptr<Shader>& fragmentShader);
	ptr<VertexDeclaration> GetVertexDeclaration();
	const std::vector<Uniform>& GetUniforms() const;
	const std::vector<std::pair<ptr<UniformBlock>, GLuint>> GetUniformBlocks() const;

private:
	void ParseProgram();
	VertexAttribute GetVertexAttribute(const std::string& name, GLenum glType);
	Uniform GetUniform(const std::string& name, GLenum glType, GLint size);

	std::vector<Uniform> m_uniforms;
	std::vector<std::pair<ptr<UniformBlock>, GLuint>> m_uniformBlocks;
	ptr<VertexDeclaration> m_vertexDeclaration;
	sptr<Shader> m_vertexShader;
	sptr<Shader> m_fragmentShader;
};

} // namespace RED_LILIUM_NAMESPACE
