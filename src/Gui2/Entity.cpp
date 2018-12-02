#include "pch.h"

#include "Entity.h"
#include "Component.h"

using namespace RED_LILIUM_NAMESPACE;

Entity::Entity(ptr<Component> owner, const std::vector<StyleId>& styles)
	: m_owner(owner)
	, m_styles(styles)
	, m_components()
{}

void Entity::DumpToJson(json& j)
{
	for (auto&[componentType, component] : m_components)
	{
		component->DumpToJson(j);
	}
}
