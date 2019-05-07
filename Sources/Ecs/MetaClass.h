#pragma once

#include <Core/Common.h>
#include "ComponentContainer.h"

namespace RED_LILIUM_NAMESPACE
{
	
class Scene;

class EntityGroupData final
{
public:
	EntityGroupData();
	~EntityGroupData();

	template<class TComponent>
	bool HasComponent() const;

	template<class ...TComponents>
	bool HasComponents() const;

	template<class TComponent>
	ptr<TComponent> GetComponent(u32 index);

	template<class ...TComponents>
	std::tuple<ptr<TComponents>...> GetComponents(u32 index);

	template<class TComponent>
	ptr<const TComponent> GetComponent(u32 index) const;

	template<class ...TComponents>
	std::tuple<ptr<const TComponents>...> GetComponents(u32 index) const;

	const ComponentsSet& GetComponentsSet() const;

	const std::vector<Entity>& GetEntities() const;

	u32 GetEntitiesCount() const;

	void SwapComponents(u32 index1, u32 index2);

	void PopComponents();

	u32 PushEmptyEntity(Entity entity);

	template<class TComponent>
	void MoveComponents(ptr<EntityGroupData> other, TComponent&& addedComponent);

	void MoveComponents(ptr<EntityGroupData> other);

	template<class TComponent>
	void InitComponentByAdding(ptr<EntityGroupData> parent);

	template<class TComponent>
	void InitComponentByRemoving(ptr<EntityGroupData> parent);

private:
	void InitComponents(ptr<EntityGroupData> parent);

	std::vector<Entity> m_entities;
	std::unordered_map<ComponentTypeId, uptr<ComponentContainerBase>> m_components;
	ComponentsSet m_componentsSet;
};

} // namespace RED_LILIUM_NAMESPACE

#include "MetaClass.hpp"
