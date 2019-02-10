#include "pch.h"
#include "MouseKeyDown.h"

using namespace RED_LILIUM_NAMESPACE;

MouseKeyDown::MouseKeyDown()
	: Event()
	, m_inputState()
	, m_mouseKey()
{}

MouseKeyDown::MouseKeyDown(const InputState& inputState, MouseKey mouseKey)
	: Event()
	, m_inputState(inputState)
	, m_mouseKey(mouseKey)
{}

void MouseKeyDown::Serialize(ptr<Serializator> serializator) const
{}

void MouseKeyDown::Deserialize(ptr<const Serializator> serializator)
{}
