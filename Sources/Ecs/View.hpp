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
inline typename View<TComponents...>::Iterator View<TComponents...>::begin()
{
	return Iterator(this, m_groups.begin());
}

template<class ...TComponents>
inline typename View<TComponents...>::Iterator View<TComponents...>::end()
{
	return Iterator(this, m_groups.end());
}

} // namespace RED_LILIUM_NAMESPACE
