#pragma once

#include <Core/Common.h>
#include "ComponentContainer.h"

namespace RED_LILIUM_NAMESPACE
{

template<class TComponent>
bool EntityGroupData::HasComponent() const
{
	return m_components.find(GetComponentTypeId<TComponent>()) != m_components.end();
}

template<class ...TComponents>
bool EntityGroupData::HasComponents() const
{
	 return (HasComponent<TComponents>() && ...);
}

template<class TComponent>
ptr<TComponent> EntityGroupData::GetComponent(u32 index)
{
	auto i = m_components.find(GetComponentTypeId<TComponent>());
	if (i == m_components.end())
	{
		return nullptr;
	}

	ptr<ComponentContainerBase> base = i->get();
	ptr<ComponentContainer<TComponent>> casted = static_cast<ptr<ComponentContainer<TComponent>>>(base);
	return casted.m_components[index];
}

template<class ...TComponents>
std::tuple<ptr<TComponents>...> EntityGroupData::GetComponents(u32 index)
{
	return std::tuple<TComponents&...>{ GetComponent<TComponents>(index)... };
}

template<class TComponent>
ptr<const TComponent> EntityGroupData::GetComponent(u32 index) const
{
	auto i = m_components.find(GetComponentTypeId<TComponent>());
	if (i == m_components.end())
	{
		return nullptr;
	}

	ptr<const ComponentContainerBase> base = i->get();
	ptr<const ComponentContainer<TComponent>> casted = static_cast<ptr<const ComponentContainer<TComponent>>>(base);
	return casted.m_components[index];
}

template<class ...TComponents>
std::tuple<ptr<const TComponents>...> EntityGroupData::GetComponents(u32 index) const
{
	return std::tuple<TComponents&...>{ GetComponent<TComponents>(index)... };
}

template<class TComponent>
void EntityGroupData::MoveComponents(ptr<EntityGroupData> other, TComponent&& addedComponent)
{
	auto i = m_components.find(GetComponentTypeId<TComponent>());
	RED_LILIUM_ASSERT(i != m_components.end());
	ptr<ComponentContainerBase> base = i->get();
	ptr<ComponentContainer<TComponent>> casted = static_cast<ptr<ComponentContainer<TComponent>>>(base);
	casted->PushComponent(std::move(addedComponent));
	MoveComponents(other);
}

template<class TComponent>
void EntityGroupData::InitComponentByAdding(ptr<EntityGroupData> parent)
{
	RED_LILIUM_ASSERT(m_componentsSet.find(GetComponentTypeId<TComponent>()) == m_componentsSet.end());
	InitComponents(parent);
	m_components.insert({ GetComponentTypeId<TComponent>(), umake<ComponentContainer<TComponent>>() });
	m_componentsSet.insert(GetComponentTypeId<TComponent>());
}

template<class TComponent>
void EntityGroupData::InitComponentByRemoving(ptr<EntityGroupData> parent)
{
	RED_LILIUM_ASSERT(m_componentsSet.find(GetComponentTypeId<TComponent>()) != m_componentsSet.end());
	InitComponents(parent);
	m_components.erase(GetComponentTypeId<TComponent>());
	m_componentsSet.erase(GetComponentTypeId<TComponent>());
}

} // namespace RED_LILIUM_NAMESPACE
