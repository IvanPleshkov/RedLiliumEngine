#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

enum class UniformType : u8
{
	Int1			= 1 << 0, // Int, used for samplers only.
	Vec4			= 1 << 1,
	Mat3			= 1 << 2,
	Mat4			= 1 << 3,
};

enum class ShaderType : u8
{
	Vertex			= 1 << 0,
	Fragment		= 1 << 1,
};

class Shader : public GpuResource
{
public:
	Shader(ptr<RenderDevice> renderDevice);
	~Shader() override;

	void CompileFromString(ShaderType type, const std::string& shader, const std::string& includeFilename = "");

private:
	ShaderType m_type;
};


class ShaderProgram : public GpuResource
{
public:
	ShaderProgram(ptr<RenderDevice> renderDevice);
	~ShaderProgram() override;

	void Link(const sptr<Shader>& vertexShader, const sptr<Shader>& fragmentShader);
	ptr<VertexDeclaration> GetVertexDeclaration();

private:
	void GetNames(std::vector<VertexAttribute>& verts, std::vector<std::string>& uniforms);
	VertexAttribute GetVertexAttribute(const std::string& name, GLenum glType);

	ptr<VertexDeclaration> m_vertexDeclaration;
	sptr<Shader> m_vertexShader;
	sptr<Shader> m_fragmentShader;
};

} // namespace RED_LILIUM_NAMESPACE
