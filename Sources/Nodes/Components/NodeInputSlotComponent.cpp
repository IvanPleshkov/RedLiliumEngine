#include "pch.h"
#include "NodeInputSlotComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodeInputSlotComponent::NodeInputSlotComponent()
	: Component()
{
}

NodeInputSlotComponent::NodeInputSlotComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodeInputSlotComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodeInputSlotComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}
