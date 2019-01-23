#pragma once

#include <Scene/Component.h>
#include <Editor/Action.h>
#include "../NodesEditor.h"

namespace RED_LILIUM_NAMESPACE
{

class ZoomEditorByScroll : public Action<NodesEditor>
{
public:
	ZoomEditorByScroll(ptr<NodesEditor> editor);
	~ZoomEditorByScroll() override = default;

	EventHandleResultFlags HandleEvent(ptr<Event> event) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
