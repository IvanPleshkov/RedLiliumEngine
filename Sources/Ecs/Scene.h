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
	std::vector<EntityGeneration> m_entityGenerations;
	std::vector<u32> m_entityMetaClass;
	std::vector<u32> m_entityMetaIndex;
	std::vector<MetaData> m_metaData;
	std::unordered_set<Entity> m_freeEntities;
};

} // namespace RED_LILIUM_NAMESPACE

#include "Scene.hpp"
