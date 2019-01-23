#include "pch.h"
#include "NodeSlotsConnectionComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodeSlotsConnectionComponent::NodeSlotsConnectionComponent()
	: Component()
{
}

NodeSlotsConnectionComponent::NodeSlotsConnectionComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodeSlotsConnectionComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodeSlotsConnectionComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}
