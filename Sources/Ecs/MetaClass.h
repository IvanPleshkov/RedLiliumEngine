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

	template<class TComponent>
	MetaData(ptr<MetaData> parent)
	{
		InitComponents(parent);
		m_components.insert({ GetComponentTypeId<TComponent>(), umake<ComponentContainer<TComponent>>() });
		m_componentsSet.insert(GetComponentTypeId<TComponent>());
	}

	MetaData(ptr<MetaData> parent, ComponentTypeId removedComponent)
	{
		RED_LILIUM_ASSERT(m_componentsSet.find(removedComponent) != m_componentsSet.end());
		InitComponents(parent);
		m_components.erase(removedComponent);
		m_componentsSet.erase(removedComponent);
	}

	template<class TComponent>
	bool HasComponent() const
	{
		return m_components.find(GetComponentTypeId<TComponent>()) != m_components.end();
	}

	template<class ...TComponents>
	bool HasComponents() const
	{
		 return (HasComponent<TComponents>() && ...);
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
		return std::tuple<TComponents&...>{ GetComponent<TComponents>(index)... };
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
		return std::tuple<TComponents&...>{ GetComponent<TComponents>(index)... };
	}

	const std::unordered_set<ComponentTypeId>& GetComponentsSet() const
	{
		return m_componentsSet;
	}

	const std::vector<Entity>& GetEntities() const
	{
		return m_entities;
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

	void MoveComponents(ptr<MetaData> other, u32 otherIndex, u32 index)
	{
		RED_LILIUM_ASSERT(other != nullptr);
		RED_LILIUM_ASSERT(otherIndex < other->m_entities.size());
		RED_LILIUM_ASSERT(index < m_entities.size());
		m_entities[index] = other->m_entities[otherIndex];
		for (auto&[componentId, otherComponents] : other->m_components)
		{
			auto it = m_components.find(componentId);
			if (it != m_components.end())
			{
				it->second->MoveComponents(otherComponents.get(), otherIndex, index);
			}
		}
	}

	void Resize(u32 newSize)
	{
		m_entities.resize(newSize);
		for (auto&[k, v] : m_components)
		{
			v->Resize(newSize);
		}
	}

private:
	void InitComponents(ptr<MetaData> parent)
	{
		for (auto&[k, v] : parent->m_components)
		{
			m_components.insert({ k, std::move(v->CreateWithSameType()) });
		}
		m_componentsSet = parent->m_componentsSet;
	}

	std::vector<Entity> m_entities;
	std::unordered_map<ComponentTypeId, uptr<ComponentContainerBase>> m_components;
	std::unordered_set<ComponentTypeId> m_componentsSet;
};

} // namespace RED_LILIUM_NAMESPACE
