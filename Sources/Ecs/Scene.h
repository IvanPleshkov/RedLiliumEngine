#pragma once

#include <Core/Common.h>
#include <Core/Flags.h>
#include <Reflection/Serialization.h>
#include "Entity.h"

namespace RED_LILIUM_NAMESPACE
{

class Component;

using ComponentTypeId = u32;

ComponentTypeId GenerateComponentTypeId()
{
	static ComponentTypeId id = 0;
	return id++;
}

template<class T>
ComponentTypeId GetComponentTypeId()
{
	static ComponentTypeId id = GenerateComponentTypeId();
	return id;
}

ComponentTypeId GenerateSystemTypeId()
{
	static ComponentTypeId id = 0;
	return id++;
}

template<class T>
ComponentTypeId GetSystemTypeId()
{
	static ComponentTypeId id = GenerateSystemTypeId();
	return id;
}

class ComponentContainerBase
{
public:
	virtual ~ComponentContainerBase() = default;

private:
	std::type_index m_componentTypeIndex;
};

template<class TComponent>
class ComponentContainer : ComponentContainerBase
{
public:
	~ComponentContainer() override = default;

private:
	std::deque<TComponent> m_components;
};

class MetaType
{};

class Scene final
{
public:
	Scene();
	~Scene();

public: // Entity
	Entity Add();
	void Add(Entity entity);
	void Remove(Entity entity);
	bool Exists(Entity entity) const;


public: // Component
	template<class TComponent, class... Args>
	TComponent& AddComponent(Entity entity, Args&&... args);

	template<class TComponent>
	void RemoveComponent(Entity entity);

	template<class ...TComponents>
	void RemoveComponents(Entity entity);

	template<class TComponent>
	bool HasComponent(Entity entity) const;

	template<class ...TComponents>
	bool HasComponents(Entity entity) const;

	template<class TComponent>
	TComponent& GetComponent(Entity entity);

	template<class ...TComponents>
	std::tuple<TComponents&...> GetComponents(Entity entity);

	template<class TComponent>
	ptr<const TComponent> GetComponent(Entity entity) const;

	template<class ...TComponents>
	std::tuple<const TComponents&...> GetComponents(Entity entity) const;

public: // System
	template<class TSystem, class... Args>
	TSystem& AddSystem(Args&&... args);

	template<class TSystem>
	void RemoveSystem();

	template<class TSystem>
	bool HasSystem() const;

	template<class TSystem>
	ptr<TSystem> GetSystem();

	template<class TSystem>
	ptr<const TSystem> GetSystem() const;

private:
	std::vector<Entity> m_entities;


	std::unordered_map<std::type_index, u32> m_componentsIndex;
	std::vector<std::vector<uptr<Component>>> m_components;
	std::vector<std::vector<Entity>> m_children;
	std::vector<Entity> m_parents;
	std::vector<std::string> m_names;
	std::vector<bool> m_enable;
};


template<class T>
inline ptr<const T> Scene::GetComponent(Entity entity) const
{
	RED_LILIUM_ASSERT(m_componentsIndex.find(std::type_index(typeid(T))) != m_componentsIndex.end());

	if (!entity.IsValid() || entity.m_index >= m_components.size())
	{
		return nullptr;
	}

	const auto& components = m_components[entity.m_index];
	const auto componentIndex = m_componentsIndex.find(std::type_index(typeid(T)));

	return Cast<const T>(components[componentIndex]);
}

} // namespace RED_LILIUM_NAMESPACE
