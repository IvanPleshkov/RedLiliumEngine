#pragma once

#include <Core/Common.h>
#include "Entity.h"
#include "MetaClass.h"

namespace RED_LILIUM_NAMESPACE
{

class ViewBase
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

	public:
		Iterator(ptr<ViewBase> viewBase, GroupsIterator groupsIterator)
			: m_viewBase(viewBase)
			, m_groupsIterator(groupsIterator)
			, m_index(0)
		{}

		Iterator operator++()
		{
			Iterator iterator(m_viewBase, m_groupsIterator);
			iterator.m_index++;
			if (iterator.m_index == iterator.m_groupsIterator->GetEntitiesCount())
			{
				iterator.m_groupsIterator++;
				iterator.m_index = 0;
			}
			return iterator;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_viewBase != other.m_viewBase ||
				m_groupsIterator != other.m_groupsIterator ||
				m_index != other.m_index;
		}

		bool operator==(const Iterator& other) const
		{
			return m_viewBase == other.m_viewBase && 
				m_groupsIterator == other.m_groupsIterator &&
				m_index == other.m_index;
		}

		ValueType operator*()
		{
			return Get();
		}

		ValueType operator->()
		{
			return Get();
		}

	private:
		ValueType Get()
		{
			return std::tuple_cat(
				m_groupsIterator->GetComponentsSet()[m_index],
				m_groupsIterator->GetComponents(m_index));
		}

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
