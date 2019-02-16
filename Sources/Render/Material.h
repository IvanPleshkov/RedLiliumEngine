#pragma once

#include "RenderCommon.h"
#include "Uniform.h"

namespace RED_LILIUM_NAMESPACE
{

class Material : public RedLiliumObject
{
public:
	Material(ptr<RenderDevice> renderDevice, std::string_view filename);
	~Material() override;

	template<class T = Material>
	static sptr<T> Create(ptr<RenderDevice> renderDevice, std::string_view filename)
	{
		return smake<T>(renderDevice, filename);
	}

	template<class T>
	bool Set(std::string_view name, T& value);

	const std::string& GetName() const;

public:
	template <class T>
	struct Handler
	{
		Handler(ptr<Material> material, std::string_view name)
			: m_uniform(nullptr)
		{
			auto i = material->m_nameToUniform.find(name);
			if (i != material->m_nameToUniform.end())
			{
				m_uniform = &material->m_uniforms[i->second];
			}
		}

		Handler(ptr<Material> material, std::string_view name, const T& defaultValue)
			: Handler(material, name)
		{
			if (m_uniform != nullptr)
			{
				m_uniform->Set(defaultValue);
			}
		}

		Handler& operator=(const T& value)
		{
			if (m_uniform != nullptr)
			{
				m_uniform->Set(value);
			}
			return *this;
		}

		ptr<Uniform> m_uniform;
	};

	using Sampler = Handler<sptr<GpuTexture>>;
	using Float = Handler<float>;
	using Vec2 = Handler<vec2>;
	using Vec3 = Handler<vec3>;
	using Vec4 = Handler<vec4>;
	using Mat2 = Handler<mat2>;
	using Mat3 = Handler<mat3>;
	using Mat4 = Handler<mat4>;

RED_LILIUM_INTERNAL:
	void Use(ptr<RenderContext> context);
	const sptr<ShaderProgram>& GetShaderProgram();

	template<class T>
	friend struct Handler;

private:
	void SetShaderProgram(const sptr<ShaderProgram>& shaderProgram);

	std::vector<Uniform> m_uniforms;
	std::map<std::string, size_t, std::less<>> m_nameToUniform;
	sptr<ShaderProgram> m_shaderProgram;
};

template<class T>
inline bool Material::Set(std::string_view name, T & value)
{
	auto i = m_nameToUniform.find(name);
	if (i == m_nameToUniform.end())
	{
		return false;
	}
	m_uniforms[i->second].Set(value);
	return true;
}

} // namespace RED_LILIUM_NAMESPACE
