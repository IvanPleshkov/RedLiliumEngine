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
	struct Empty
	{
		bool operator ==(const Empty&) const { return true; }
		bool operator !=(const Empty&) const { return false; }
	};

	using ValueVariants = std::variant<
		Empty,
		u64,
		sptr<GpuTexture>,
		f32,
		vec2,
		vec3,
		vec4,
		mat2,
		mat3,
		mat4>;

	Uniform(std::string_view name, UniformType type, u64 location);

	void Set(const ValueVariants& value);
	void Set(const sptr<GpuTexture>& value);
	void Set(u64 value);
	void Set(f32 value);
	void Set(const vec2& value);
	void Set(const vec3& value);
	void Set(const vec4& value);
	void Set(const mat2& value);
	void Set(const mat3& value);
	void Set(const mat4& value);

	bool HasValue() const;
	bool IsSampler() const;
	const ValueVariants& GetValue() const { return m_value; }

	void Apply() const;
	void SendToBlock(ptr<UniformBlock> block) const;

	const std::string& GetName() const { return m_name; }
	u64 GetLocation() const { return m_location; }
	UniformType GetType() const { return m_type; }

	bool operator ==(const Uniform& u) const;
	bool operator !=(const Uniform& u) const;

	static UniformType GlTypeToUniformType(GLenum glType);

private:
	ValueVariants m_value;

	std::string m_name;
	UniformType m_type;
	u64 m_location;
};

class UniformBlock : public GpuBuffer
{
public:
	UniformBlock(ptr<ShaderProgram> program, std::string_view name);
	~UniformBlock() override;

	void Check(ptr<ShaderProgram> program);
	void SendData();

/*internal*/ public:
	void SetData(const void* data, size_t size, size_t offset);
	std::vector<Uniform> FindUniformsFromShader(ptr<ShaderProgram> program) const;

	friend class RenderDevice;
private:
	std::vector<Uniform> m_uniforms;
	std::vector<char> m_data;
};

} // namespace RED_LILIUM_NAMESPACE
