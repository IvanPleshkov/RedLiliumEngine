#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

using namespace RED_LILIUM_NAMESPACE;

Scene::Scene()
{
	m_metaData.push_back(umake<MetaData>());
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

	m_entityMetaClass[entity.m_index] = GetComponentsFreeMetaData();
	m_entityMetaIndex[entity.m_index] = GetComponentsFreeMetaData()->PushEmptyEntity(entity);

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
	m_entityMetaClass[entity.m_index] = GetComponentsFreeMetaData();
	m_entityMetaIndex[entity.m_index] = GetComponentsFreeMetaData()->PushEmptyEntity(entity);
}

void Scene::Remove(Entity entity)
{
	RED_LILIUM_ASSERT(Exists(entity));
	m_entityGenerations[entity.m_index] = 0;
	m_freeEntities.insert(entity);
	
	ptr<MetaData> oldData = m_entityMetaClass[entity.m_index];
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());
	oldData->PopComponents();
	CheckEmptyMetaData(oldData);

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

void Scene::CheckEmptyMetaData(ptr<MetaData> metaData)
{
	if (!metaData->GetEntities().empty() || metaData->GetComponentsSet().empty())
	{
		return;
	}
}

ptr<MetaData> Scene::GetComponentsFreeMetaData()
{
	return m_metaData.front().get();
}

size_t Scene::ComponentsSetHash(const Scene::ComponentsSet& set)
{
	size_t result;
	for (const auto id : set)
	{
		result ^= id;
	}
	return result;
}
