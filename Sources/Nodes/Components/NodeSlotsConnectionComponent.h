#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeSlotsConnectionComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodeSlotsConnectionComponent, Component);

	NodeSlotsConnectionComponent();
	NodeSlotsConnectionComponent(ptr<Entity> parent);
	~NodeSlotsConnectionComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
