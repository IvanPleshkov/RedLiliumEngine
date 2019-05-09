#include "pch.h"
#include "Scene.h"
#include "Entity.h"

using namespace RED_LILIUM_NAMESPACE;

Scene::Scene()
{
	auto entityEmptyGroupData = umake<EntityGroupData>();
	m_entityEmptyGroupData = entityEmptyGroupData.get();
	m_entityGroupData.insert({ m_entityEmptyGroupData, std::move(entityEmptyGroupData) });
	m_metaClasses.insert({ m_entityEmptyGroupData->GetComponentsSet(), m_entityEmptyGroupData });
}

Scene::~Scene()
{}

Entity Scene::Add()
{
	Entity entity;

	if (m_freeEntities.empty())
	{
		entity.m_chunk = 0;
		entity.m_generation = 1;
		entity.m_index = m_entityGenerations.size();
		m_entityGenerations.push_back(entity.m_generation);
		m_entityMetaClass.push_back(nullptr);
		m_entityMetaIndex.push_back(u32_max);
	}
	else
	{
		auto entityIterator = m_freeEntities.begin();
		entity = *entityIterator;
		m_freeEntities.erase(entityIterator);

		if (entity.m_generation == maxEntityGeneration)
		{
			return Add();
		}

		entity.m_generation++;
		RED_LILIUM_ASSERT(m_entityGenerations[entity.m_index] == 0);
		m_entityGenerations[entity.m_index] = entity.m_generation;
	}

	m_entityMetaClass[entity.m_index] = m_entityEmptyGroupData;
	m_entityMetaIndex[entity.m_index] = m_entityEmptyGroupData->PushEmptyEntity(entity);

	return entity;
}

void Scene::Add(Entity entity)
{
	RED_LILIUM_ASSERT(entity.m_index < m_entityGenerations.size());
	RED_LILIUM_ASSERT(m_entityGenerations[entity.m_index] == 0);
	auto entityIterator = m_freeEntities.find(entity);
	if (entityIterator != m_freeEntities.end())
	{
		m_freeEntities.erase(entityIterator);
	}
	m_entityGenerations[entity.m_index] = entity.m_generation;
	m_entityMetaClass[entity.m_index] = m_entityEmptyGroupData;
	m_entityMetaIndex[entity.m_index] = m_entityEmptyGroupData->PushEmptyEntity(entity);
}

void Scene::Remove(Entity entity)
{
	RED_LILIUM_ASSERT(Exists(entity));
	m_freeEntities.insert(entity);
	
	ptr<EntityGroupData> oldData = m_entityMetaClass[entity.m_index];
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());
	oldData->PopComponents();
	CheckEmptyEntityGroupData(oldData);

	m_entityGenerations[entity.m_index] = 0;
	m_entityMetaClass[entity.m_index] = nullptr;
	m_entityMetaIndex[entity.m_index] = u32_max;
}

bool Scene::Exists(Entity entity) const
{
	if (entity.m_index >= m_entityGenerations.size())
	{
		return false;
	}
	return m_entityGenerations[entity.m_index] == entity.m_generation;
}

void Scene::SwapEntitiesInsideGroup(Entity entity1, Entity entity2)
{
	RED_LILIUM_ASSERT(Exists(entity1) && Exists(entity2));
	RED_LILIUM_ASSERT(m_entityMetaClass[entity1.m_index] == m_entityMetaClass[entity2.m_index]);

	if (entity1 == entity2)
	{
		return;
	}

	m_entityMetaClass[entity1.m_index]->SwapComponents(
		m_entityMetaIndex[entity1.m_index],
		m_entityMetaIndex[entity2.m_index]);
	std::swap(m_entityMetaIndex[entity1.m_index], m_entityMetaIndex[entity2.m_index]);
}

void Scene::CheckEmptyEntityGroupData(ptr<EntityGroupData> entityGroupData)
{
	return;

	if (!entityGroupData->GetEntities().empty() || entityGroupData->GetComponentsSet().empty())
	{
		return;
	}

	for (auto&[key, value] : m_views)
	{
		value->OnRemoveGroup(entityGroupData);
	}
}
