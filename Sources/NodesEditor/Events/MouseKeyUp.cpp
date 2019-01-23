#include "pch.h"
#include "MouseKeyUp.h"

using namespace RED_LILIUM_NAMESPACE;

MouseKeyUp::MouseKeyUp()
	: Event()
	, m_mouseState()
	, m_mouseKey()
{}

MouseKeyUp::MouseKeyUp(MouseState mouseState, MouseKey key)
	: Event()
	, m_mouseState(mouseState)
	, m_mouseKey(key)
{}

void MouseKeyUp::Serialize(ptr<Serializator> serializator) const
{}

void MouseKeyUp::Deserialize(ptr<const Serializator> serializator)
{}
