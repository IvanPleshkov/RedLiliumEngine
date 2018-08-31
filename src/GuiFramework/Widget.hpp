#pragma once

namespace RED_LILIUM_NAMESPACE
{

template <class TWidget>
ptr<TWidget> Widget::Add()
{
	ptr<TWidget> result = nullptr;
	if (IsAddingChildExists<TWidget>())
	{
		result = Cast<TWidget>(m_children[m_childAddingCursor]);
		if (result)
		{
			++m_childAddingCursor;
		}
	}

	if (result == nullptr)
	{
		result = AddNewChild<TWidget>();
	}
	result->BeginChildAdding();
	return result;
}


template <class TWidget>
bool Widget::IsAddingChildExists()
{
	if (m_childAddingCursor >= m_children.size())
	{
		m_childAddingCursor = m_children.size();
		return false;
	}

	std::type_index widgetTypeIndex = std::type_index(typeid(TWidget));
	ptr<Widget> cursorChild = m_children[m_childAddingCursor].get();
	std::type_index cursorChildTypeIndex = cursorChild->GetWidgetTypeIndex();
	return widgetTypeIndex == cursorChildTypeIndex;
}


template <class TWidget>
ptr<TWidget> Widget::AddNewChild()
{
	// remove obsolete childs
	m_children.erase(m_children.begin() + m_childAddingCursor, m_children.end());

	uptr<TWidget> widget = umake<TWidget>();
	ptr<TWidget> result = widget.get();
	result->m_parent = this;
	result->m_panel = m_panel;
	m_children.push_back(std::move(widget));

	// move cursor to end of childs
	m_childAddingCursor = m_children.size();
	return result;
}

}  // namespace RED_LILIUM_NAMESPACE
