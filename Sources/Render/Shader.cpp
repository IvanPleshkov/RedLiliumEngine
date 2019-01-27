#include "pch.h"
#include "Shader.h"

using namespace RED_LILIUM_NAMESPACE;

Shader::Shader(ptr<RenderDevice> renderDevice)
	: GpuResource(renderDevice)
{
}

Shader::~Shader()
{
}

void Shader::CompileFromString(ShaderType type, const std::string& shader, const std::string& includeFilename)
{
	m_type = type;
	switch (type)
	{
	case ShaderType::Vertex:
		m_handler = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderType::Fragment:
		m_handler = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		RED_LILIUM_NOT_IMPLEMENTED();
	}

	const char* c_str = shader.c_str();
	glShaderSource(m_handler, 1, &c_str, NULL);
	glCompileShader(m_handler);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(m_handler, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_handler, 512, NULL, infoLog);
		std::string errorMessage = infoLog;
		RED_LILIUM_LOG_ERROR("Shader Compilation Error: " + errorMessage);
		RED_LILIUM_ASSERT("Shader Compilation Error!");
	}
}

void Shader::Compile(ShaderType type, const std::string& shaderFilename)
{
	RED_LILIUM_NOT_IMPLEMENTED();
}

ShaderProgram::ShaderProgram(ptr<RenderDevice> renderDevice)
	: GpuResource(renderDevice)
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Link(const sptr<Shader>& vertexShader, const  sptr<Shader>& fragmentShader)
{
	m_vertexShader = vertexShader;
	m_fragmentShader = fragmentShader;

	m_handler = glCreateProgram();
	glAttachShader(m_handler, vertexShader->GetNative());
	glAttachShader(m_handler, fragmentShader->GetNative());
	glLinkProgram(m_handler);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(m_handler, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_handler, 512, NULL, infoLog);
		std::string errorMessage = infoLog;
		RED_LILIUM_LOG_ERROR("Shader Compilation Error: " + errorMessage);
		RED_LILIUM_ASSERT("Shader Compilation Error!");
	}
}
