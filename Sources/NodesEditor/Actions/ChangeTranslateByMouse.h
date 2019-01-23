#pragma once

#include <Scene/Component.h>
#include <Editor/Action.h>
#include "../NodesEditor.h"

namespace RED_LILIUM_NAMESPACE
{

class ChangeTranslateByMouse : public Action<NodesEditor>
{
public:
	ChangeTranslateByMouse(ptr<NodesEditor> editor);
	~ChangeTranslateByMouse() override = default;

	EventHandleResultFlags HandleEvent(ptr<Event> event) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
