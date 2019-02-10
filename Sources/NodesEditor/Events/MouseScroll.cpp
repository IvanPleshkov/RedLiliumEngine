#include "pch.h"
#include "MouseScroll.h"
#include "../Actions/ZoomEditorByScroll.h"

using namespace RED_LILIUM_NAMESPACE;

MouseScroll::MouseScroll()
	: Event()
	, m_inputState()
	, m_scrollDelta()
{
}

MouseScroll::MouseScroll(const InputState& inputState, float scrollDelta)
	: Event()
	, m_inputState(inputState)
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

const InputState& MouseScroll::GetMouseState() const
{
	return m_inputState;
}

float MouseScroll::GetScrollDelta() const
{
	return m_scrollDelta;
}
