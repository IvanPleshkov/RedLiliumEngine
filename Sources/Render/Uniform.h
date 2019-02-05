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
	Uniform(const std::string& name, GLenum glType, u64 location);

	void Set(const sptr<GpuTexture>& value);
	void Set(f32 value);
	void Set(const vec2& value);
	void Set(const vec3& value);
	void Set(const vec4& value);
	void Set(const mat2& value);
	void Set(const mat3& value);
	void Set(const mat4& value);

	void Apply();
	void SendToBlock(ptr<UniformBlock> block);

	const std::string& GetName() const { return m_name; }
	u64 GetLocation() const { return m_location; }

	bool operator ==(const Uniform& u) const;
	bool operator !=(const Uniform& u) const;

private:
	using ValueVariants = std::variant<
		sptr<GpuTexture>,
		f32,
		vec2,
		vec3,
		vec4,
		mat2,
		mat3,
		mat4>;

	ValueVariants m_value;

	std::string m_name;
	UniformType m_type;
	u64 m_location;
};

class UniformBlock : public GpuBuffer
{
public:
	UniformBlock(ptr<ShaderProgram> program, const std::string& name);
	~UniformBlock() override;

	void Check(ptr<ShaderProgram> program);
	void SendData();
	const std::vector<Uniform>& GetUniforms() const;
	const std::string& GetName() const { return m_name; }

public: // only for struct Uniform
	void SetData(const void* data, size_t size, size_t offset);
	std::vector<Uniform> FindUniformsFromShader(ptr<ShaderProgram> program) const;

private:
	std::vector<Uniform> m_uniforms;
	std::string m_name;
	std::vector<char> m_data;
};

} // namespace RED_LILIUM_NAMESPACE
