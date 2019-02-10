#include "pch.h"
#include "MouseMove.h"

using namespace RED_LILIUM_NAMESPACE;

MouseMove::MouseMove()
	: Event()
	, m_inputState()
	, m_prevMousePos()
{}

MouseMove::MouseMove(const InputState& inputState, vec2 prevMousePos)
	: Event()
	, m_inputState(inputState)
	, m_prevMousePos(prevMousePos)
{}

void MouseMove::Serialize(ptr<Serializator> serializator) const
{}

void MouseMove::Deserialize(ptr<const Serializator> serializator)
{}
