#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{


enum class UniformType : u8
{
	Sampler1D,
	Sampler2D,
	Sampler3D,
	Float,
	Vec2,
	Vec3,
	Vec4,
	Mat2,
	Mat3,
	Mat4,
};

struct Uniform
{
	Uniform(const std::string& name, UniformType type, u64 location);

	void Set(f32 value);
	void Set(const vec2& value);
	void Set(const vec3& value);
	void Set(const vec4& value);
	void Set(const mat2& value);
	void Set(const mat3& value);
	void Set(const mat4& value);

	void Apply();

	const std::string& GetName() const { return m_name; }
	u64 GetLocation() const { return m_location; }

private:
	union
	{
		f32 m_f32;
		vec2 m_vec2;
		vec3 m_vec3;
		vec4 m_vec4;
		mat2 m_mat2;
		mat3 m_mat3;
		mat4 m_mat4;
	} m_value;

	std::string m_name;
	UniformType m_type;
	u64 m_location;
};

} // namespace RED_LILIUM_NAMESPACE
