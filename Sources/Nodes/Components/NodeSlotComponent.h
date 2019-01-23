#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeSlotComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodeSlotComponent, Component);

	NodeSlotComponent();
	NodeSlotComponent(ptr<Entity> parent);
	~NodeSlotComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
