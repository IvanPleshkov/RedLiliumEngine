#pragma once

#include "RenderCommon.h"
#include "Uniform.h"

namespace RED_LILIUM_NAMESPACE
{

class RenderContext : public RedLiliumObject
{
public:
	RenderContext(ptr<RenderDevice> renderDevice);
	~RenderContext();

	void Draw(const sptr<GpuMesh>& mesh, const sptr<Material>& material);

	// todo: use string_view because most of uniform names in code are const char*
	template<class T>
	bool Set(const std::string& name, T& value);

private:
	void UpdateUniformBlocks();

	ptr<RenderDevice> m_renderDevice;
	std::vector<std::pair<Uniform, ptr<UniformBlock>>> m_uniformBlocksToUpdate;
};

template<class T>
inline bool RenderContext::Set(const std::string& name, T& value)
{
	auto u = m_renderDevice->GetGlobalUniform(name);
	if (u.has_value())
	{
		m_uniformBlocksToUpdate.push_back(u.value());
		m_uniformBlocksToUpdate.back().first.Set(value);
		return true;
	}
	return false;
}

} // namespace RED_LILIUM_NAMESPACE
