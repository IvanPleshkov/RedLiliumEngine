#pragma once

#include <Core/Common.h>
#include "ComponentContainer.h"

namespace RED_LILIUM_NAMESPACE
{
	
class Scene;

class MetaData final
{
public:
	MetaData()
	{}

	MetaData(ptr<MetaData> parent, uptr<ComponentContainerBase> componentContainer)
	{
		InitComponents(parent);
		m_components.insert({ componentContainer->GetComponentTypeId(), std::move(componentContainer) });
	}

	MetaData(ptr<MetaData> parent, ComponentTypeId removedComponent)
	{
		InitComponents(parent);
		m_components.erase(removedComponent);
	}

	template<class TComponent>
	bool HasComponent() const
	{
		return m_components.find(GetComponentTypeId<TComponent>()) != m_components.end();
	}

	template<class ...TComponents>
	bool HasComponents() const
	{
		 return (HasComponents<TComponents>() && ...);
	}

	template<class TComponent>
	ptr<TComponent> GetComponent(u32 index)
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
	std::tuple<ptr<TComponents>...> GetComponents(u32 index)
	{
		return std::tuple<TComponents&...>{ GetComponents<TComponents>(index)... };
	}

	template<class TComponent>
	ptr<const TComponent> GetComponent(u32 index) const
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
	std::tuple<ptr<const TComponents>...> GetComponents(u32 index) const
	{
		return std::tuple<TComponents&...>{ GetComponents<TComponents>(index)... };
	}

	void SwapComponents(u32 index1, u32 index2)
	{
		std::swap(m_entities[index1], m_entities[index2]);
		for (auto&[k, v] : m_components)
		{
			v->SwapComponents(index1, index2);
		}
	}

	void Invalidate(u32 index)
	{
		m_entities[index] = Entity();
		for (auto&[k, v] : m_components)
		{
			v->Invalidate(index);
		}
	}

	template<class TComponent>
	void SetComponent(u32 index, TComponent component)
	{
		auto i = m_components.find(GetComponentTypeId<TComponent>());
		RED_LILIUM_ASSERT(i != m_components.end());
		ptr<ComponentContainerBase> base = i->get();
		ptr<ComponentContainer<TComponent>> casted = static_cast<ptr<ComponentContainer<TComponent>>>(base);
		casted->m_components[index] = std::move(component);
	}

private:
	void InitComponents(ptr<MetaData> parent)
	{
		for (auto&[k, v] : parent->m_components)
		{
			m_components.insert({ k, std::move(v->CreateWithSameType()) });
		}
	}

	std::vector<Entity> m_entities;
	std::unordered_map<ComponentTypeId, uptr<ComponentContainerBase>> m_components;
};

} // namespace RED_LILIUM_NAMESPACE
