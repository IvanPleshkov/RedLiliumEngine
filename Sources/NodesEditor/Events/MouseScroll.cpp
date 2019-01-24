#include "pch.h"
#include "MouseScroll.h"
#include "../Actions/ZoomEditorByScroll.h"

using namespace RED_LILIUM_NAMESPACE;

MouseScroll::MouseScroll()
	: Event()
	, m_mouseState()
	, m_scrollDelta()
{
}

MouseScroll::MouseScroll(MouseState mouseState, float scrollDelta)
	: Event()
	, m_mouseState(mouseState)
	, m_scrollDelta(scrollDelta)
{
}

void MouseScroll::Serialize(ptr<Serializator> serializator) const
{
}

void MouseScroll::Deserialize(ptr<const Serializator> serializator)
{
}

uptr<ActionBase> MouseScroll::TriggerAction(ptr<Editor> editor)
{
	return umake<ZoomEditorByScroll>(editor);
}

MouseState MouseScroll::GetMouseState() const
{
	return m_mouseState;
}

float MouseScroll::GetScrollDelta() const
{
	return m_scrollDelta;
}
