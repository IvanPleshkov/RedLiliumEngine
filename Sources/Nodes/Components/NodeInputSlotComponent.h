#pragma once

#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class NodeInputSlotComponent : public Component
{
public:
	RED_LILIUM_CLASS(NodeInputSlotComponent, Component);

	NodeInputSlotComponent();
	NodeInputSlotComponent(ptr<Entity> parent);
	~NodeInputSlotComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
