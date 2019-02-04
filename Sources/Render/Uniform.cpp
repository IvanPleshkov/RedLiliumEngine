#include "pch.h"
#include "Uniform.h"
#include "Shader.h"

using namespace RED_LILIUM_NAMESPACE;

Uniform::Uniform(const std::string& name, GLenum glType, u64 location)
	: m_name(name)
	, m_location(location)
{
	switch (glType)
	{
	case GL_SAMPLER_1D:
		m_type = UniformType::Sampler1D;
		break;
	case GL_SAMPLER_2D:
		m_type = UniformType::Sampler2D;
		break;
	case GL_SAMPLER_3D:
		m_type = UniformType::Sampler3D;
		break;
	case GL_FLOAT:
		m_type = UniformType::Float;
		break;
	case GL_FLOAT_VEC2:
		m_type = UniformType::Vec2;
		break;
	case GL_FLOAT_VEC3:
		m_type = UniformType::Vec3;
		break;
	case GL_FLOAT_VEC4:
		m_type = UniformType::Vec4;
		break;
	case GL_FLOAT_MAT2:
		m_type = UniformType::Mat2;
		break;
	case GL_FLOAT_MAT3:
		m_type = UniformType::Mat3;
		break;
	case GL_FLOAT_MAT4:
		m_type = UniformType::Mat4;
		break;
	default:
		m_type = UniformType::Float;
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
	}
}

void Uniform::Set(f32 value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Float);
	m_value.m_f32 = value;
}

void Uniform::Set(const vec2& value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Vec2);
	m_value.m_vec2 = value;
}

void Uniform::Set(const vec3& value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Vec3);
	m_value.m_vec3 = value;
}

void Uniform::Set(const vec4& value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Vec4);
	m_value.m_vec4 = value;
}

void Uniform::Set(const mat2& value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Mat2);
	m_value.m_mat2 = value;
}

void Uniform::Set(const mat3& value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Mat3);
	m_value.m_mat3 = value;
}

void Uniform::Set(const mat4& value)
{
	RED_LILIUM_ASSERT(m_type == UniformType::Mat3);
	m_value.m_mat4 = value;
}

void Uniform::Apply()
{
	switch (m_type)
	{
	case UniformType::Sampler1D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Sampler2D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Sampler3D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Float:
		glUniform1f(m_location, m_value.m_f32);
		break;
	case UniformType::Vec2:
		glUniform2f(m_location, m_value.m_vec2.x, m_value.m_vec2.y);
		break;
	case UniformType::Vec3:
		glUniform3f(m_location, m_value.m_vec3.x, m_value.m_vec3.y, m_value.m_vec3.z);
		break;
	case UniformType::Vec4:
		glUniform4f(m_location, m_value.m_vec4.x, m_value.m_vec4.y, m_value.m_vec4.z, m_value.m_vec4.w);
		break;
	case UniformType::Mat2:
		glUniformMatrix2fv(m_location, 1, GL_FALSE, &m_value.m_mat2[0][0]);
		break;
	case UniformType::Mat3:
		glUniformMatrix3fv(m_location, 1, GL_FALSE, &m_value.m_mat3[0][0]);
		break;
	case UniformType::Mat4:
		glUniformMatrix4fv(m_location, 1, GL_FALSE, &m_value.m_mat4[0][0]);
		break;
	default:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	}
}

void Uniform::SendToBlock(ptr<UniformBlock> block)
{
	switch (m_type)
	{
	case UniformType::Sampler1D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Sampler2D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Sampler3D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Float:
		block->SetData(&m_value.m_f32, sizeof(float), static_cast<size_t>(m_location));
		break;
	case UniformType::Vec2:
		block->SetData(&m_value.m_vec2, sizeof(vec2), static_cast<size_t>(m_location));
		break;
	case UniformType::Vec3:
		block->SetData(&m_value.m_vec3, sizeof(vec3), static_cast<size_t>(m_location));
		break;
	case UniformType::Vec4:
		block->SetData(&m_value.m_vec4, sizeof(vec4), static_cast<size_t>(m_location));
		break;
	case UniformType::Mat2:
		block->SetData(&m_value.m_mat2, sizeof(mat2), static_cast<size_t>(m_location));
		break;
	case UniformType::Mat3:
		block->SetData(&m_value.m_mat3, sizeof(mat3), static_cast<size_t>(m_location));
		break;
	case UniformType::Mat4:
		block->SetData(&m_value.m_mat4, sizeof(mat4), static_cast<size_t>(m_location));
		break;
	default:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	}
}

bool Uniform::operator ==(const Uniform& u) const
{
	if (m_name != u.m_name || m_type != u.m_type || m_location != m_location)
	{
		return false;
	}

	switch (m_type)
	{
	case UniformType::Sampler1D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Sampler2D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Sampler3D:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	case UniformType::Float:
		return m_value.m_f32 == u.m_value.m_f32;
	case UniformType::Vec2:
		return m_value.m_vec2 == u.m_value.m_vec2;
	case UniformType::Vec3:
		return m_value.m_vec3 == u.m_value.m_vec3;
	case UniformType::Vec4:
		return m_value.m_vec4 == u.m_value.m_vec4;
	case UniformType::Mat2:
		return m_value.m_mat2 == u.m_value.m_mat2;
	case UniformType::Mat3:
		return m_value.m_mat3 == u.m_value.m_mat3;
	case UniformType::Mat4:
		return m_value.m_mat4 == u.m_value.m_mat4;
	default:
		RED_LILIUM_ASSERT(false && "Unsupported uniform type!");
		break;
	}

	return true;
}

bool Uniform::operator !=(const Uniform& u) const
{
	return !(*this == u);
}

UniformBlock::UniformBlock(ptr<ShaderProgram> program, const std::string& name)
	: GpuBuffer(program->GetRenderDevice(), GL_UNIFORM_BUFFER, GpuBufferUsage::Dynamic)
	, m_name(name)
	, m_data()
{
	auto blockIndex = glGetUniformBlockIndex(program->GetNative(), m_name.c_str());

	GLint blockSize;
	glGetActiveUniformBlockiv(program->GetNative(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	m_data.resize(blockSize, char(0));

	m_uniforms = FindUniformsFromShader(program);
}

UniformBlock::~UniformBlock()
{ }

void UniformBlock::Check(ptr<ShaderProgram> program)
{
	auto uniforms = FindUniformsFromShader(program);
	std::map<std::string, size_t> nameToIndex;
	for (size_t i = 0; i < uniforms.size(); i++)
	{
		nameToIndex.insert({ uniforms[i].GetName(), i });
	}

	for (const auto& uniform : m_uniforms)
	{
		auto it = nameToIndex.find(uniform.GetName());
		if (it == nameToIndex.end())
		{
			RED_LILIUM_ASSERT(false && "Invalid Uniform Block. Uniform block is different in another shader");
		}

		size_t index = it->second;
		nameToIndex.erase(it);

		const Uniform& checkUniform = uniforms[index];

		if (uniform != checkUniform)
		{
			RED_LILIUM_ASSERT(false && "Invalid Uniform Block. Uniform block is different in another shader");
		}
	}

	if (!nameToIndex.empty())
	{
		RED_LILIUM_ASSERT(false && "Invalid Uniform Block. Uniform block is different in another shader");
	}
}

void UniformBlock::SendData()
{
	Bind();
	GpuBuffer::SendData(m_data.data(), m_data.size());
	Undind();
}

const std::vector<Uniform>& UniformBlock::GetUniforms() const
{
	return m_uniforms;
}

void UniformBlock::SetData(void* data, size_t size, size_t offset)
{
	std::memcpy(m_data.data() + offset, data, size);
}

std::vector<Uniform> UniformBlock::FindUniformsFromShader(ptr<ShaderProgram> program) const
{
	auto blockIndex = glGetUniformBlockIndex(program->GetNative(), m_name.c_str());

	GLint uniformsCount;
	const GLsizei bufSize = 64; // maximum name length
	GLchar uniformName[bufSize]; // variable name in GLSL
	GLsizei length; // name length
	auto programHandler = program->GetNative();
	glGetProgramiv(programHandler, GL_ACTIVE_UNIFORMS, &uniformsCount);
	std::vector<std::string> uniformNames;
	std::vector<GLenum> uniformTypes;
	for (GLuint i = 0; i < uniformsCount; i++)
	{
		GLint uniformBlock;
		glGetActiveUniformsiv(programHandler, 1, &i, GL_UNIFORM_BLOCK_INDEX, &uniformBlock);
		if (uniformBlock != blockIndex)
		{
			continue;
		}

		GLint size;
		GLenum type;
		glGetActiveUniform(programHandler, i, bufSize, &length, &size, &type, uniformName);

		uniformNames.push_back(std::string(uniformName));
		uniformTypes.push_back(type);
	}

	std::vector<GLint> offsets;
	offsets.resize(uniformNames.size(), 0);

	std::vector<GLuint> indices;
	indices.resize(uniformNames.size(), 0);
	{
		std::vector<const char*> cNames;
		cNames.resize(uniformNames.size(), nullptr);
		for (size_t i = 0; i < uniformNames.size(); i++)
		{
			cNames[i] = uniformNames[i].c_str();
		}

		glGetUniformIndices(
			programHandler,
			static_cast<GLsizei>(uniformNames.size()),
			cNames.data(),
			indices.data());

		glGetActiveUniformsiv(
			programHandler,
			static_cast<GLsizei>(uniformNames.size()),
			indices.data(),
			GL_UNIFORM_OFFSET,
			offsets.data());
	}

	std::vector<Uniform> result;
	result.reserve(uniformNames.size());
	for (size_t i = 0; i < uniformNames.size(); i++)
	{
		Uniform u(uniformNames[i], uniformTypes[i], static_cast<u64>(offsets[i]));
		result.push_back(std::move(u));
	}
	return std::move(result);
}
