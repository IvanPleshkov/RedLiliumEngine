#include "Entity.h"
#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Scene.h"

using namespace RED_LILIUM_NAMESPACE;

Entity::Entity(ptr<Entity> parent)
	: m_parent(parent)
	, m_scene(parent->GetScene())
{
}

Entity::Entity(ptr<Scene> scene)
	: m_parent(nullptr)
	, m_scene(scene)
{
}

void Entity::Serialize(ptr<Serializator> serializator) const
{
}

void Entity::Deserialize(ptr<const Serializator> serializator)
{
}

ptr<Scene> Entity::GetScene()
{
	return m_scene;
}

ptr<const Scene> Entity::GetScene() const
{
	return m_scene;
}

ptr<Entity> Entity::GetParent()
{
	return m_parent;
}

ptr<const Entity> Entity::GetParent() const
{
	return m_parent;
}

u64 Entity::GetChildrenCount() const
{
	return m_children.size();
}

ptr<Entity> Entity::GetChild(u64 index)
{
	RED_LILIUM_ASSERT(index < m_children.size());

	return m_children[index].get();
}

ptr<const Entity> Entity::GetChild(u64 index) const
{
	RED_LILIUM_ASSERT(index < m_children.size());

	return m_children[index].get();
}
