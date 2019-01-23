#include "pch.h"
#include "ZoomEditorByScroll.h"

using namespace RED_LILIUM_NAMESPACE;

ZoomEditorByScroll::ZoomEditorByScroll(ptr<NodesEditor> editor)
	: Action(editor)
{
}

EventHandleResultFlags ZoomEditorByScroll::HandleEvent(ptr<Event> event)
{
	return Action::HandleEvent(event);
}
