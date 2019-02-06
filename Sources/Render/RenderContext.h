#pragma once

#include "RenderCommon.h"
#include "Uniform.h"
#include <Core/Camera.h>

namespace RED_LILIUM_NAMESPACE
{

class RenderContext : public RedLiliumObject
{
public:
	RenderContext(ptr<RenderDevice> renderDevice);
	~RenderContext();

	void Draw(const sptr<GpuMesh>& mesh, const sptr<Material>& material);

	template<class T>
	bool Set(std::string_view name, const T& value);

	void CurrentCamera(const Camera& camera);
	const Camera& CurrentCamera() const { return m_currentCamera; }

	void Clear();

/*internal*/ public:
	ptr<const Uniform::ValueVariants> GetUniformValue(std::string_view name);

private:
	void UpdateUniformBlocks();

	ptr<RenderDevice> m_renderDevice;
	Camera m_currentCamera;
	std::vector<std::pair<ptr<Uniform>, ptr<UniformBlock>>> m_uniformBlocksToUpdate;
	std::map<std::string, Uniform::ValueVariants, std::less<>> m_contextUniforms;
};

template<class T>
inline bool RenderContext::Set(std::string_view name, const T& value)
{
	auto u = m_renderDevice->GetGlobalUniform(name);
	if (u.has_value())
	{
		m_uniformBlocksToUpdate.push_back(u.value());
		m_uniformBlocksToUpdate.back().first->Set(value);
		return true;
	}
	auto it = m_contextUniforms.find(name);
	if (it != m_contextUniforms.end())
	{
		m_contextUniforms.erase(it);
	}
	m_contextUniforms.insert({ std::string(name), value });
	return false;
}

} // namespace RED_LILIUM_NAMESPACE
