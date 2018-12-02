#include "pch.h"

#include "Component.h"

using namespace RED_LILIUM_NAMESPACE;

Component::Component(ptr<Entity> entity)
	: m_entity(entity)
{}

ptr<Entity> Component::GetEntity()
{
	return m_entity;
}

ptr<const Entity> Component::GetEntity() const
{
	return m_entity;
}
