#pragma once

#include <Core/Common.h>
#include "Entity.h"
#include "MetaClass.h"

namespace RED_LILIUM_NAMESPACE
{

class ViewBase : public NonCopyable
{
public:
	ViewBase();
	virtual ~ViewBase();
	void OnAddGroup(ptr<EntityGroupData> group);
	void OnRemoveGroup(ptr<EntityGroupData> group);
	const ComponentsSet& GetComponentsSet() const;

protected:
	using GroupsIterator = std::unordered_set<ptr<EntityGroupData>>::iterator;

	ComponentsSet m_componentsSet;
	std::unordered_set<ptr<EntityGroupData>> m_groups;
};

template<class ...TComponents>
class View : public ViewBase
{
public:
	class Iterator
	{
	public:
		using ValueType = std::tuple<Entity, ptr<TComponents>...>;

		Iterator(ptr<ViewBase> viewBase, GroupsIterator groupsIterator, u32 index);
		Iterator operator++();
		bool operator!=(const Iterator& other) const;
		bool operator==(const Iterator& other) const;
		ValueType operator*();
		ValueType operator->();

	private:
		ValueType Get();

	private:
		ptr<ViewBase> m_viewBase;
		GroupsIterator m_groupsIterator;
		u32 m_index;
	};

public:
	View();
	~View() override;

	Iterator begin();
	Iterator end();
};

} // namespace RED_LILIUM_NAMESPACE

#include "View.hpp"
