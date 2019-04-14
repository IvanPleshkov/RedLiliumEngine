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
	return false;
}

template<class ...TComponents>
inline bool Scene::HasComponents(Entity entity) const
{
	return false;
}

template<class TComponent>
inline ptr<TComponent> Scene::GetComponent(Entity entity)
{
	return ptr<TComponent>();
}

template<class ...TComponents>
inline std::tuple<ptr<TComponents>...> Scene::GetComponents(Entity entity)
{
	return std::tuple<ptr<TComponents>...>();
}

template<class TComponent>
inline ptr<const TComponent> Scene::GetComponent(Entity entity) const
{
	return ptr<const TComponent>();
}

template<class ...TComponents>
inline std::tuple<ptr<const TComponents>...> Scene::GetComponents(Entity entity) const
{
	return std::tuple<ptr<const TComponents>...>();
}

template<class TComponent>
inline ComponentContainer<TComponent>& Scene::GetComponentContainer()
{
}

template<class TComponent>
inline const ComponentContainer<TComponent>& Scene::GetComponentContainer() const
{

}

} // namespace RED_LILIUM_NAMESPACE
