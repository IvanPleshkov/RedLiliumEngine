#include "pch.h"
#include "NodesViewComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodesViewComponent::NodesViewComponent()
	: Component()
{
}

NodesViewComponent::NodesViewComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodesViewComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodesViewComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}
