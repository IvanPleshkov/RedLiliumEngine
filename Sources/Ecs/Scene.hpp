#include "Scene.h"
#pragma once

namespace RED_LILIUM_NAMESPACE
{

template<class TComponent, class ...Args>
inline ptr<TComponent> Scene::AddComponent(Entity entity, Args && ...args)
{
	RED_LILIUM_ASSERT(HasComponent<TComponent>(entity) == false);

	ptr<EntityGroupData> oldData = m_entityMetaClass[entity.m_index];
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());

	ptr<EntityGroupData> newData = nullptr;
	{
		ComponentsSet newComponentsSet = oldData->GetComponentsSet();
		RED_LILIUM_ASSERT(newComponentsSet.find(GetComponentTypeId<TComponent>()) == newComponentsSet.end());
		newComponentsSet.insert(GetComponentTypeId<TComponent>());
		auto it = m_metaClasses.find(newComponentsSet);
		if (it == m_metaClasses.end())
		{
			newData = CreateEntityGroupDataByAddComponent<TComponent>(oldData);
		}
		else
		{
			newData = it->second;
		}
	}

	newData->MoveComponents(oldData, TComponent(std::forward<Args>(args)...));
	m_entityMetaClass[entity.m_index] = newData;
	m_entityMetaIndex[entity.m_index] = newData->GetEntities().size() - 1;
	CheckEmptyEntityGroupData(oldData);

	return GetComponent<TComponent>(entity);
}

template<class TComponent>
inline void Scene::RemoveComponent(Entity entity)
{
	if (!HasComponent<TComponent>(entity))
	{
		return;
	}

	ptr<EntityGroupData> oldData = m_entityMetaClass[entity.m_index];
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());

	ptr<EntityGroupData> newData = nullptr;
	{
		ComponentsSet newComponentsSet = oldData->GetComponentsSet();
		RED_LILIUM_ASSERT(newComponentsSet.find(GetComponentTypeId<TComponent>()) != newComponentsSet.end());
		newComponentsSet.erase(GetComponentTypeId<TComponent>());
		auto it = m_metaClasses.find(newComponentsSet);
		if (it == m_metaClasses.end())
		{
			newData = CreateEntityGroupDataByRemoveComponent<TComponent>(oldData);
		}
		else
		{
			newData = it->second;
		}
	}

	newData->MoveComponents(oldData);
	m_entityMetaClass[entity.m_index] = newData;
	m_entityMetaIndex[entity.m_index] = newData->GetEntities().size() - 1;
	CheckEmptyEntityGroupData(oldData);
}

template<class ...TComponents>
inline void Scene::RemoveComponents(Entity entity)
{
	RemoveComponent<TComponents>(entity)...;
}

template<class TComponent>
inline bool Scene::HasComponent(Entity entity) const
{
	return m_entityMetaClass[entity.m_index]->HasComponent<TComponent>();
}

template<class ...TComponents>
inline bool Scene::HasComponents(Entity entity) const
{
	return m_entityMetaClass[entity.m_index]->HasComponents<TComponents...>();
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

template<class ...TComponents>
inline View<TComponents...>& Scene::View()
{
	return View<TComponents...>();
}

template<class TComponent>
inline ptr<EntityGroupData> Scene::CreateEntityGroupDataByAddComponent(ptr<EntityGroupData> entityGroupData)
{
	uptr<EntityGroupData> newMetaData = umake<EntityGroupData>();
	newMetaData->InitComponentByAdding<TComponent>(entityGroupData);
	ptr<EntityGroupData> result = newMetaData.get();
	m_metaClasses.insert({ newMetaData->GetComponentsSet(), result });
	m_entityGroupData.insert({ result, std::move(newMetaData) });
	return result;
}

template<class TComponent>
inline ptr<EntityGroupData> Scene::CreateEntityGroupDataByRemoveComponent(ptr<EntityGroupData> entityGroupData)
{
	uptr<EntityGroupData> newMetaData = umake<EntityGroupData>();
	newMetaData->InitComponentByRemoving<TComponent>(entityGroupData);
	ptr<EntityGroupData> result = newMetaData.get();
	m_metaClasses.insert({ newMetaData->GetComponentsSet(), result });
	m_entityGroupData.insert({ result, std::move(newMetaData) });
	return result;
}

} // namespace RED_LILIUM_NAMESPACE
