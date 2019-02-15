#pragma once

#include <Render/Material.h>
#include "../Assimp/AssimpMaterial.h"

namespace RED_LILIUM_NAMESPACE
{

class SimpleForwardTexturedOpaque : public Material, public IAssimpMaterial
{
public:
	SimpleForwardTexturedOpaque(ptr<RenderDevice> renderDevice)
		: Material(renderDevice, "Shaders\\SimpleForwardTexturedOpaque\\material.json")
		, m_diffuse(this, "g_diffuse")
		, m_specular(this, "g_specular")
	{}
	~SimpleForwardTexturedOpaque() override = default;

	Sampler m_diffuse;
	Sampler m_specular;

public: // IAssimpMaterial
	u8 GetRequiredDiffuseTexture() override { return 1; }
	void SetDiffuseTexture(sptr<GpuTexture> texture, u8 index) override
	{
		m_diffuse = texture;
	}

	u8 GetRequiredSpecularTexture() override { return 1; }
	void SetSpecularTexture(sptr<GpuTexture> texture, u8 index) override
	{
		m_specular = texture;
	}
};

} // namespace RED_LILIUM_NAMESPACE
