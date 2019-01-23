#include "pch.h"
#include "ChangeTranslateByMouse.h"

using namespace RED_LILIUM_NAMESPACE;

ChangeTranslateByMouse::ChangeTranslateByMouse(ptr<NodesEditor> editor)
	: Action(editor)
{}

EventHandleResultFlags ChangeTranslateByMouse::HandleEvent(ptr<Event> event)
{
	return Action::HandleEvent(event);
}
