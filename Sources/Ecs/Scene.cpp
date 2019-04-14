#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

using namespace RED_LILIUM_NAMESPACE;

Scene::Scene()
{}

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

	if (m_entityMetaClass.size() >= entity.m_index)
	{
		m_entityMetaClass.resize(entity.m_index + 1);
	}
	m_entityMetaClass[entity.m_index] = 0;

	if (m_entityMetaIndex.size() >= entity.m_index)
	{
		m_entityMetaIndex.resize(entity.m_index + 1);
	}
	m_entityMetaIndex[entity.m_index] = 0;
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
	m_entityMetaClass[entity.m_index] = 0;
	m_entityMetaIndex[entity.m_index] = 0;
}

void Scene::Remove(Entity entity)
{
	RED_LILIUM_ASSERT(Exists(entity));
	m_entityGenerations[entity.m_index] = 0;
	m_freeEntities.insert(entity);
}

bool Scene::Exists(Entity entity) const
{
	if (entity.m_index >= m_entityGenerations.size())
	{
		return false;
	}
	return m_entityGenerations[entity.m_index] == entity.m_generation;
}
