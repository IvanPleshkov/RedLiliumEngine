#pragma once

#include <Core/Common.h>
#include <ComponentContainer.h>

namespace RED_LILIUM_NAMESPACE
{
	
class Scene;

class MetaData final
{
public:
	MetaData(Scene& scene);
	MetaData(Scene& scene, ptr<MetaData> parent, ComponentTypeId addedType);
	MetaData(Scene& scene, ptr<MetaData> parent, ComponentTypeId removedType);

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

private:
	std::unordered_map<ComponentTypeId, uptr<ComponentContainerBase>> m_components;
	std::vector<Entity> m_entities;
};

} // namespace RED_LILIUM_NAMESPACE
