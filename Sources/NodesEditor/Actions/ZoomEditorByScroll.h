#pragma once

#include <Scene/Component.h>
#include <Editor/Action.h>
#include "../NodesEditor.h"

namespace RED_LILIUM_NAMESPACE
{

class ZoomEditorByScroll : public Action<NodesEditor>
{
public:
	ZoomEditorByScroll(ptr<Editor> editor);
	~ZoomEditorByScroll() override = default;

	bool NeedPushToUndoStack() const override { return false; }
	EventHandleResultFlags HandleEvent(ptr<Event> event) override;

private:
	void Zoom(float value);
};

} // namespace RED_LILIUM_NAMESPACE
