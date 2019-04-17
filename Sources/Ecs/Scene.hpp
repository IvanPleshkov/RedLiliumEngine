#include "Scene.h"
#pragma once

namespace RED_LILIUM_NAMESPACE
{

template<class TComponent, class ...Args>
inline ptr<TComponent> Scene::AddComponent(Entity entity, Args && ...args)
{
	RED_LILIUM_ASSERT(HasComponent<TComponent>(entity) == false);

	ptr<MetaData> oldData = m_metaData[m_entityMetaClass[entity.m_index]].get();
	SwapEntitiesInsideGroup(entity, oldData->GetEntities().back());

	ptr<MetaData> newData = nullptr;
	{
		ComponentsSet newComponentsSet = oldData->GetComponentsSet();
		newComponentsSet.insert(GetComponentTypeId<TComponent>());
		auto it = m_metaClasses.find(newComponentsSet);
		if (it == m_metaClasses.end())
		{

		}
		else
		{
			newData = it->second;
		}
	}

	return nullptr;
}

template<class TComponent>
inline void Scene::RemoveComponent(Entity entity)
{
	if (!HasComponent<TComponent>(entity))
	{
		return;
	}


}

template<class ...TComponents>
inline void Scene::RemoveComponents(Entity entity)
{
	RemoveComponent<TComponents>(entity)...;
}

template<class TComponent>
inline bool Scene::HasComponent(Entity entity) const
{
	auto& metaData = m_metaData[m_entityMetaClass[entity.m_index]];
	return metaData.HasComponent<TComponent>(m_entityMetaIndex[entity.m_index]);
}

template<class ...TComponents>
inline bool Scene::HasComponents(Entity entity) const
{
	auto& metaData = m_metaData[m_entityMetaClass[entity.m_index]];
	return metaData.HasComponents<TComponents...>(m_entityMetaIndex[entity.m_index]);
}

template<class TComponent>
inline ptr<TComponent> Scene::GetComponent(Entity entity)
{
	auto& metaData = m_metaData[m_entityMetaClass[entity.m_index]];
	return metaData.GetComponent<TComponent>(m_entityMetaIndex[entity.m_index]);
}

template<class ...TComponents>
inline std::tuple<ptr<TComponents>...> Scene::GetComponents(Entity entity)
{
	auto& metaData = m_metaData[m_entityMetaClass[entity.m_index]];
	return metaData.GetComponents<TComponents...>(m_entityMetaIndex[entity.m_index]);
}

template<class TComponent>
inline ptr<const TComponent> Scene::GetComponent(Entity entity) const
{
	auto& metaData = m_metaData[m_entityMetaClass[entity.m_index]];
	return metaData.GetComponent<TComponent>(m_entityMetaIndex[entity.m_index]);
}

template<class ...TComponents>
inline std::tuple<ptr<const TComponents>...> Scene::GetComponents(Entity entity) const
{
	auto& metaData = m_metaData[m_entityMetaClass[entity.m_index]];
	return metaData.GetComponents<TComponents...>(m_entityMetaIndex[entity.m_index]);
}

} // namespace RED_LILIUM_NAMESPACE
