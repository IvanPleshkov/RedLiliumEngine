#include "pch.h"
#include "ZoomEditorByScroll.h"
#include "../Events/MouseScroll.h"

using namespace RED_LILIUM_NAMESPACE;

ZoomEditorByScroll::ZoomEditorByScroll(ptr<Editor> editor)
	: Action(editor)
{
	m_name = "Zoom";
}

EventHandleResultFlags ZoomEditorByScroll::HandleEvent(ptr<Event> event)
{
	if (auto e = Cast<MouseScroll>(event))
	{
		Zoom(e->GetScrollDelta());

		EventHandleResultFlags result;
		result.Add(EventHandleResult::ActionFinished);
		result.Add(EventHandleResult::EventHandled);
		return result;
	}
	return Action::HandleEvent(event);
}

void ZoomEditorByScroll::Zoom(float value)
{
	auto scene = m_editor->GetEditableScene();
	auto root = scene->GetRoot();
	auto nodesRoot = root->GetChild("Nodes");
	auto viewComponent = nodesRoot->GetComponent<NodesViewComponent>();
	
	auto oldScale = viewComponent->GetScale();
	auto newScale = oldScale + value / 5.0f;
	viewComponent->SetScale(newScale);
}
