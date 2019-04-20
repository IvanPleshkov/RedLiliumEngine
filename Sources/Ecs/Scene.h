#pragma once

#include <Core/Common.h>
#include <Core/Flags.h>
#include <Reflection/Serialization.h>
#include "Entity.h"
#include "MetaClass.h"

namespace RED_LILIUM_NAMESPACE
{

class Component;

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
	ptr<TComponent> AddComponent(Entity entity, Args&&... args);

	template<class TComponent>
	void RemoveComponent(Entity entity);

	template<class ...TComponents>
	void RemoveComponents(Entity entity);

	template<class TComponent>
	bool HasComponent(Entity entity) const;

	template<class ...TComponents>
	bool HasComponents(Entity entity) const;

	template<class TComponent>
	ptr<TComponent> GetComponent(Entity entity);

	template<class ...TComponents>
	std::tuple<ptr<TComponents>...> GetComponents(Entity entity);

	template<class TComponent>
	ptr<const TComponent> GetComponent(Entity entity) const;

	template<class ...TComponents>
	std::tuple<ptr<const TComponents>...> GetComponents(Entity entity) const;

public: // Views

private:
	void SwapEntitiesInsideGroup(Entity e1, Entity e2);
	void CheckEmptyEntityGroupData(ptr<EntityGroupData> entityGroupData);

	template<class TComponent>
	ptr<EntityGroupData> CreateEntityGroupDataByAddComponent(ptr<EntityGroupData> entityGroupData);

	template<class TComponent>
	ptr<EntityGroupData> CreateEntityGroupDataByRemoveComponent(ptr<EntityGroupData> entityGroupData);

	using ComponentsSet = std::unordered_set<ComponentTypeId>;
	static size_t ComponentsSetHash(const ComponentsSet& set);

private:
	// entity data
	std::vector<EntityGeneration> m_entityGenerations;
	std::vector<ptr<EntityGroupData>> m_entityMetaClass;
	std::vector<u32> m_entityMetaIndex;
	std::unordered_set<Entity> m_freeEntities;

	// components data
	ptr<EntityGroupData> m_entityEmptyGroupData;
	std::unordered_map<ptr<EntityGroupData>, uptr<EntityGroupData>> m_entityGroupData;
	std::unordered_map<ComponentsSet, ptr<EntityGroupData>, decltype(&ComponentsSetHash)> m_metaClasses;

	// views data
};

} // namespace RED_LILIUM_NAMESPACE

#include "Scene.hpp"
