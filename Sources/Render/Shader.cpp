#include "pch.h"
#include "Shader.h"
#include "RenderDevice.h"

using namespace RED_LILIUM_NAMESPACE;

Shader::Shader(ptr<RenderDevice> renderDevice, std::string_view resourceName)
	: GpuResource(renderDevice, resourceName)
{
}

Shader::~Shader()
{
	if (m_handler != 0)
	{
		glDeleteShader(m_handler);
		m_handler = 0;
	}
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

ShaderProgram::ShaderProgram(ptr<RenderDevice> renderDevice, std::string_view resourceName)
	: GpuResource(renderDevice, resourceName)
	, m_vertexDeclaration(nullptr)
	, m_vertexShader(nullptr)
	, m_fragmentShader(nullptr)
{ }

ShaderProgram::~ShaderProgram()
{
	if (m_handler != 0)
	{
		glDeleteProgram(m_handler);
		m_handler = 0;
	}
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

	ParseProgram();
}

ptr<VertexDeclaration> ShaderProgram::GetVertexDeclaration()
{
	return m_vertexDeclaration;
}

const std::vector<Uniform>& ShaderProgram::GetUniforms() const
{
	return m_uniforms;
}

const std::vector<std::pair<ptr<UniformBlock>, GLuint>> ShaderProgram::GetUniformBlocks() const
{
	return m_uniformBlocks;
}

void ShaderProgram::ParseProgram()
{
	GLint count;
	const GLsizei bufSize = 64; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	std::vector<VertexInput> attributes;
	glGetProgramiv(m_handler, GL_ACTIVE_ATTRIBUTES, &count);
	for (GLuint i = 0; i < count; i++)
	{
		GLint size;
		GLenum type;
		glGetActiveAttrib(m_handler, i, bufSize, &length, &size, &type, name);

		std::string s(name);
		std::transform(s.begin(), s.end(), s.begin(), std::tolower);
		VertexInput vertexInput;
		vertexInput.layout = glGetAttribLocation(m_handler, name);
		vertexInput.vertexAttribute = GetVertexAttribute(s, type);
		attributes.push_back(vertexInput);
	}
	std::sort(attributes.begin(), attributes.end(), [](const VertexInput& lhs, const VertexInput& rhs)
	{
		return lhs.layout < rhs.layout;
	});
	m_vertexDeclaration = m_renderDevice->GetVertexDeclaration(attributes);

	m_uniforms.clear();
	glGetProgramiv(m_handler, GL_ACTIVE_UNIFORMS, &count);
	for (GLuint i = 0; i < count; i++)
	{
		GLint uniformBlock;
		glGetActiveUniformsiv(m_handler, 1, &i, GL_UNIFORM_BLOCK_INDEX, &uniformBlock);
		if (uniformBlock != -1)
		{
			continue;
		}

		GLint size;
		GLenum type;
		glGetActiveUniform(m_handler, i, bufSize, &length, &size, &type, name);

		std::string s(name);
		m_uniforms.push_back(std::move(GetUniform(s, type, size)));
	}
	std::sort(m_uniforms.begin(), m_uniforms.end(), [](const Uniform& lhs, const Uniform& rhs)
	{
		return lhs.GetLocation() < rhs.GetLocation();
	});

	m_uniformBlocks.clear();
	glGetProgramiv(m_handler, GL_ACTIVE_UNIFORM_BLOCKS, &count);
	for (GLint i = 0; i < count; i++)
	{
		glGetActiveUniformBlockName(m_handler, i, bufSize, &length, name);
		std::string s(name);
		auto blockIndex = glGetUniformBlockIndex(m_handler, s.c_str());
		m_uniformBlocks.push_back({ m_renderDevice->GetUniformBlock(this, s), blockIndex });
	}
}

VertexAttribute ShaderProgram::GetVertexAttribute(const std::string& name, GLenum glType)
{
	if (name == "position")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC3);
		return VertexAttribute::Position;
	}
	else if (name == "normal")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC3);
		return VertexAttribute::Normal;
	}
	else if (name == "tangent")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC3);
		return VertexAttribute::Tangent;
	}
	else if (name == "bitangent")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC3);
		return VertexAttribute::Bitangent;
	}
	else if (name == "color0")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC4);
		return VertexAttribute::Color0;
	}
	else if (name == "color1")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC4);
		return VertexAttribute::Color1;
	}
	else if (name == "color2")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC4);
		return VertexAttribute::Color2;
	}
	else if (name == "color3")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC4);
		return VertexAttribute::Color3;
	}
	else if (name == "texcoord0")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord0;
	}
	else if (name == "texcoord1")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord1;
	}
	else if (name == "texcoord2")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord2;
	}
	else if (name == "texcoord3")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord3;
	}
	else if (name == "texcoord4")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord4;
	}
	else if (name == "texcoord5")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord5;
	}
	else if (name == "texcoord6")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord6;
	}
	else if (name == "texcoord7")
	{
		RED_LILIUM_ASSERT(glType == GL_FLOAT_VEC2);
		return VertexAttribute::TexCoord7;
	}
	RED_LILIUM_ASSERT(false && "Unsupported vertex type!");
	return VertexAttribute::Position;
}

Uniform ShaderProgram::GetUniform(const std::string& name, GLenum glType, GLint size)
{
	u64 location = glGetUniformLocation(m_handler, name.c_str());
	Uniform uniform(name, Uniform::GlTypeToUniformType(glType), location);
	return std::move(uniform);
}
