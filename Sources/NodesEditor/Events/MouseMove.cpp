#include "pch.h"
#include "MouseMove.h"

using namespace RED_LILIUM_NAMESPACE;

MouseMove::MouseMove()
	: Event()
	, m_mouseState()
	, m_prevMousePos()
{}

MouseMove::MouseMove(MouseState mouseState, vec2 prevMousePos)
	: Event()
	, m_mouseState(mouseState)
	, m_prevMousePos(prevMousePos)
{}

void MouseMove::Serialize(ptr<Serializator> serializator) const
{}

void MouseMove::Deserialize(ptr<const Serializator> serializator)
{}
