#include "pch.h"
#include "NodeOutputSlotComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodeOutputSlotComponent::NodeOutputSlotComponent()
	: Component()
{
}

NodeOutputSlotComponent::NodeOutputSlotComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodeOutputSlotComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodeOutputSlotComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}
