#include "pch.h"
#include "MouseKeyDown.h"

using namespace RED_LILIUM_NAMESPACE;

MouseKeyDown::MouseKeyDown()
	: Event()
	, m_mouseState()
	, m_mouseKey()
{}

MouseKeyDown::MouseKeyDown(MouseState mouseState, MouseKey mouseKey)
	: Event()
	, m_mouseState(mouseState)
	, m_mouseKey(mouseKey)
{}

void MouseKeyDown::Serialize(ptr<Serializator> serializator) const
{}

void MouseKeyDown::Deserialize(ptr<const Serializator> serializator)
{}
