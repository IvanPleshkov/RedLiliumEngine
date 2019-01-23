#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeOutputSlotComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodeOutputSlotComponent, Component);

	NodeOutputSlotComponent();
	NodeOutputSlotComponent(ptr<Entity> parent);
	~NodeOutputSlotComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
