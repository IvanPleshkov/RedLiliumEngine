#pragma once

#include "../PipelineCommon.h"
#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class Material;

class MeshRenderer : public Component
{
public:
	RED_LILIUM_CLASS(MeshRenderer, Component);

	MeshRenderer();
	MeshRenderer(ptr<Entity> parent);
	~MeshRenderer() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	const sptr<Material>& GetMaterial() const;
	void SetMaterial(const sptr<Material>& material);

private:
	sptr<Material> m_material;
};

} // namespace RED_LILIUM_NAMESPACE
