#include "pch.h"
#include "MouseKeyUp.h"

using namespace RED_LILIUM_NAMESPACE;

MouseKeyUp::MouseKeyUp()
	: Event()
	, m_inputState()
	, m_mouseKey()
{}

MouseKeyUp::MouseKeyUp(const InputState& inputState, MouseKey key)
	: Event()
	, m_inputState(inputState)
	, m_mouseKey(key)
{}

void MouseKeyUp::Serialize(ptr<Serializator> serializator) const
{}

void MouseKeyUp::Deserialize(ptr<const Serializator> serializator)
{}
