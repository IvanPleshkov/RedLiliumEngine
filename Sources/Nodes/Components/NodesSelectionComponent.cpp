#include "pch.h"
#include "NodesSelectionComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodesSelectionComponent::NodesSelectionComponent()
	: Component()
{
}

NodesSelectionComponent::NodesSelectionComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodesSelectionComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodesSelectionComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}
