#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Scene.h"

using namespace RED_LILIUM_NAMESPACE;

Entity::Entity()
	: m_parent(nullptr)
	, m_scene(nullptr)
{}

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

const std::string& Entity::GetName() const
{
	return m_name;
}

void Entity::SetName(const std::string& name)
{
	m_name = name;
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

ptr<Entity> Entity::GetChild(const std::string& name, u64 index)
{
	for (auto& child : m_children)
	{
		if (child->m_name == name)
		{
			if (index == 0)
			{
				return child.get();
			}
			else
			{
				index--;
			}
		}
	}
	return nullptr;
}

ptr<const Entity> Entity::GetChild(const std::string& name, u64 index) const
{
	for (const auto& child : m_children)
	{
		if (child->m_name == name)
		{
			if (index == 0)
			{
				return child.get();
			}
			else
			{
				index--;
			}
		}
	}
	return nullptr;
}

ptr<Entity> Entity::AddChild(const std::string& name, u64 position)
{
	uptr<Entity> entity = umake<Entity>(this);
	entity->SetName(name);
	ptr<Entity> result = entity.get();
	if (position == u64_max)
	{
		m_children.push_back(std::move(entity));
	}
	else
	{
		RED_LILIUM_ASSERT(position <= m_children.size());
		m_children.insert(m_children.begin() + position, std::move(entity));
	}
	return result;
}
