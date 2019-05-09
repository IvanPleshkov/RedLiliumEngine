#pragma once

namespace RED_LILIUM_NAMESPACE
{

template<class ...TComponents>
inline View<TComponents...>::View()
{
	m_componentsSet = GetComponentTypesId<TComponents...>();
}

template<class ...TComponents>
inline View<TComponents...>::~View()
{}

template<class ...TComponents>
inline View<TComponents...>::Iterator::Iterator(ptr<ViewBase> viewBase, GroupsIterator groupsIterator, u32 index)
	: m_viewBase(viewBase)
	, m_groupsIterator(groupsIterator)
	, m_index(index)
{
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator View<TComponents...>::Iterator::operator++()
{
	m_index++;
	if (m_index == (*m_groupsIterator)->GetEntitiesCount())
	{
		m_groupsIterator++;
		m_index = 0;
	}
	return *this;
}

template<class ...TComponents>
inline bool View<TComponents...>::Iterator::operator!=(const Iterator& other) const
{
	return m_viewBase != other.m_viewBase ||
		m_groupsIterator != other.m_groupsIterator ||
		m_index != other.m_index;
}

template<class ...TComponents>
inline bool View<TComponents...>::Iterator::operator==(const Iterator& other) const
{
	return m_viewBase == other.m_viewBase &&
		m_groupsIterator == other.m_groupsIterator &&
		m_index == other.m_index;
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator::ValueType View<TComponents...>::Iterator::operator*()
{
	return Get();
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator::ValueType View<TComponents...>::Iterator::operator->()
{
	return Get();
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator::ValueType View<TComponents...>::Iterator::Get()
{
	const Entity entity = (*m_groupsIterator)->GetEntities().at(static_cast<size_t>(m_index));
	return ValueType{ entity, (*m_groupsIterator)->GetComponent<TComponents>(m_index)... };
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator View<TComponents...>::begin()
{
	return Iterator(this, m_groups.begin(), 0);
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator View<TComponents...>::end()
{
	return Iterator(this, m_groups.end(), 0);
}

} // namespace RED_LILIUM_NAMESPACE
