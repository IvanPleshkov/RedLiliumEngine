#include "pch.h"
#include "NodesViewComponent.h"

using namespace RED_LILIUM_NAMESPACE;

NodesViewComponent::NodesViewComponent()
	: Component()
	, m_scale(1)
	, m_translate(0, 0)
{
}

NodesViewComponent::NodesViewComponent(ptr<Entity> parent)
	: Component(parent)
	, m_scale(1)
	, m_translate(0, 0)
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

float NodesViewComponent::GetScale() const
{
	return m_scale;
}

void NodesViewComponent::SetScale(float value)
{
	m_scale = value;
}

vec2 NodesViewComponent::GetTranslate() const
{
	return m_translate;
}

void NodesViewComponent::SetTranslate(vec2 translate)
{
	m_translate = translate;
}

vec2 NodesViewComponent::GetScreenPosition(vec2 documentPosition) const
{
	return m_scale * (m_translate + documentPosition);
}

vec2 NodesViewComponent::GetDocumentPosition(vec2 screenPosition) const
{
	return screenPosition / m_scale - m_translate;
}
