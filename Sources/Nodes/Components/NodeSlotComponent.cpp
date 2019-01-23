#include "pch.h"
#include "NodeSlotComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodeSlotComponent::NodeSlotComponent()
	: Component()
{
}

NodeSlotComponent::NodeSlotComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodeSlotComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodeSlotComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}
