#include "pch.h"
#include "Uniform.h"

using namespace RED_LILIUM_NAMESPACE;

Uniform::Uniform(const std::string& name, UniformType type, u64 location)
	: m_name(name)
	, m_type(type)
	, m_location(location)
{
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
