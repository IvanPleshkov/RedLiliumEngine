#include "Scene.h"
#pragma once

namespace RED_LILIUM_NAMESPACE
{

template<class TComponent, class ...Args>
inline ptr<TComponent> Scene::AddComponent(Entity entity, Args && ...args)
{
	RED_LILIUM_ASSERT(HasComponent<TComponent>(entity) == false);

	ptr<MetaData> oldData = m_entityMetaClass[entity.m_index];
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());

	ptr<MetaData> newData = nullptr;
	{
		ComponentsSet newComponentsSet = oldData->GetComponentsSet();
		RED_LILIUM_ASSERT(newComponentsSet.find(GetComponentTypeId<TComponent>()) == newComponentsSet.end());
		newComponentsSet.insert(GetComponentTypeId<TComponent>());
		auto it = m_metaClasses.find(newComponentsSet);
		if (it == m_metaClasses.end())
		{
			newData = CreateMetaDataByAddComponent<TComponent>(oldData);
		}
		else
		{
			newData = it->second;
		}
	}

	newData->MoveComponents(oldData, TComponent(std::forward(args...)));
	m_entityMetaClass[entity.m_index] = newData;
	m_entityMetaIndex[entity.m_index] = newData->GetEntities().size() - 1;
	CheckEmptyMetaData(oldData);

	return GetComponent<TComponent>(entity);
}

template<class TComponent>
inline void Scene::RemoveComponent(Entity entity)
{
	if (!HasComponent<TComponent>(entity))
	{
		return;
	}

	ptr<MetaData> oldData = m_entityMetaClass[entity.m_index];
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());

	ptr<MetaData> newData = nullptr;
	{
		ComponentsSet newComponentsSet = oldData->GetComponentsSet();
		RED_LILIUM_ASSERT(newComponentsSet.find(GetComponentTypeId<TComponent>()) != newComponentsSet.end());
		newComponentsSet.erase(GetComponentTypeId<TComponent>());
		auto it = m_metaClasses.find(newComponentsSet);
		if (it == m_metaClasses.end())
		{
			newData = CreateMetaDataByRemoveComponent<TComponent>(oldData);
		}
		else
		{
			newData = it->second;
		}
	}

	newData->MoveComponents(oldData);
	m_entityMetaClass[entity.m_index] = newData;
	m_entityMetaIndex[entity.m_index] = newData->GetEntities().size() - 1;
	CheckEmptyMetaData(oldData);
}

template<class ...TComponents>
inline void Scene::RemoveComponents(Entity entity)
{
	RemoveComponent<TComponents>(entity)...;
}

template<class TComponent>
inline bool Scene::HasComponent(Entity entity) const
{
	return m_entityMetaClass[entity.m_index]->HasComponent<TComponent>(m_entityMetaIndex[entity.m_index]);
}

template<class ...TComponents>
inline bool Scene::HasComponents(Entity entity) const
{
	return m_entityMetaClass[entity.m_index]->HasComponents<TComponents...>(m_entityMetaIndex[entity.m_index]);
}

template<class TComponent>
inline ptr<TComponent> Scene::GetComponent(Entity entity)
{
	return m_entityMetaClass[entity.m_index]->GetComponent<TComponent>(m_entityMetaIndex[entity.m_index]);
}

template<class ...TComponents>
inline std::tuple<ptr<TComponents>...> Scene::GetComponents(Entity entity)
{
	return m_entityMetaClass[entity.m_index]->GetComponents<TComponents...>(m_entityMetaIndex[entity.m_index]);
}

template<class TComponent>
inline ptr<const TComponent> Scene::GetComponent(Entity entity) const
{
	return m_entityMetaClass[entity.m_index]->GetComponent<TComponent>(m_entityMetaIndex[entity.m_index]);
}

template<class ...TComponents>
inline std::tuple<ptr<const TComponents>...> Scene::GetComponents(Entity entity) const
{
	return m_entityMetaClass[entity.m_index]->GetComponents<TComponents...>(m_entityMetaIndex[entity.m_index]);
}

template<class TComponent>
inline ptr<MetaData> Scene::CreateMetaDataByAddComponent(ptr<MetaData> metaData)
{
	uptr<MetaData> newMetaData = umake<MetaData>();
	newMetaData->InitComponentByAdding<TComponent>(metaData);
	ptr<MetaData> result = newMetaData.get();
	m_metaData.push_back({ newMetaData.get(), std::move(newMetaData) });
	return result;
}

template<class TComponent>
inline ptr<MetaData> Scene::CreateMetaDataByRemoveComponent(ptr<MetaData> metaData)
{
	uptr<MetaData> newMetaData = umake<MetaData>();
	newMetaData->InitComponentByRemoving<TComponent>(metaData);
	ptr<MetaData> result = newMetaData.get();
	m_metaData.push_back({ newMetaData.get(), std::move(newMetaData) });
	return result;
}

} // namespace RED_LILIUM_NAMESPACE
