#include "pch.h"
#include "Entity.h"
#include "Scene.h"
#include "MetaClass.h"

using namespace RED_LILIUM_NAMESPACE;

EntityGroupData::EntityGroupData()
{}

EntityGroupData::~EntityGroupData()
{}

const std::unordered_set<ComponentTypeId>& EntityGroupData::GetComponentsSet() const
{
	return m_componentsSet;
}

const std::vector<Entity>& EntityGroupData::GetEntities() const
{
	return m_entities;
}

void EntityGroupData::SwapComponents(u32 index1, u32 index2)
{
	std::swap(m_entities[index1], m_entities[index2]);
	for (auto&[k, v] : m_components)
	{
		v->SwapComponents(index1, index2);
	}
}

void EntityGroupData::PopComponents()
{
	m_entities.pop_back();
	for (auto&[k, v] : m_components)
	{
		v->PopComponent();
	}
}

u32 EntityGroupData::PushEmptyEntity(Entity entity)
{
	RED_LILIUM_ASSERT(m_components.empty());
	m_entities.push_back(entity);
	return static_cast<u32>(m_entities.size() - 1);
}

void EntityGroupData::MoveComponents(ptr<EntityGroupData> other)
{
	RED_LILIUM_ASSERT(other != nullptr);
	RED_LILIUM_ASSERT(!other->m_entities.empty());

	m_entities.push_back(other->m_entities.back());
	other->m_entities.pop_back();

	for (auto&[componentId, otherComponents] : other->m_components)
	{
		auto it = m_components.find(componentId);
		if (it != m_components.end())
		{
			it->second->MoveComponent(otherComponents.get());
		}
		else
		{
			otherComponents->PopComponent();
		}
	}

#if RED_LILIUM_USE_ASSERTS
	for (auto&[id, components] : other->m_components)
	{
		RED_LILIUM_ASSERT(components->Size() == other->m_entities.size());
	}
	for (auto&[id, components] : m_components)
	{
		RED_LILIUM_ASSERT(components->Size() == m_entities.size());
	}
#endif
}

void EntityGroupData::InitComponents(ptr<EntityGroupData> parent)
{
	for (auto&[k, v] : parent->m_components)
	{
		m_components.insert({ k, std::move(v->CreateWithSameType()) });
	}
	m_componentsSet = parent->m_componentsSet;
}
