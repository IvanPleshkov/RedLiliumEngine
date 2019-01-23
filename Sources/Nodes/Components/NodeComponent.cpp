#include "pch.h"
#include "NodeComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodeComponent::NodeComponent()
	: Component()
{
}

NodeComponent::NodeComponent(ptr<Entity> parent)
	: Component(parent)
{
}

void NodeComponent::Serialize(ptr<Serializator> serializator) const
{
	Component::Serialize(serializator);
}

void NodeComponent::Deserialize(ptr<const Serializator> serializator)
{
	Component::Deserialize(serializator);
}

vec2 NodeComponent::GetPosition() const
{
	return m_position;
}

void NodeComponent::SetPosition(vec2 position)
{
	m_position = position;
}

vec2 NodeComponent::GetSize() const
{
	return m_size;
}

void NodeComponent::SetSize(vec2 size)
{
	m_size = size;
}
