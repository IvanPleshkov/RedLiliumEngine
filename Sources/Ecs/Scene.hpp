#include "Scene.h"
#pragma once

namespace RED_LILIUM_NAMESPACE
{

template<class TComponent, class ...Args>
inline ptr<TComponent> Scene::AddComponent(Entity entity, Args && ...args)
{
	// TODO: insert return statement here
}

template<class TComponent>
inline void Scene::RemoveComponent(Entity entity)
{
}

template<class ...TComponents>
inline void Scene::RemoveComponents(Entity entity)
{
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
