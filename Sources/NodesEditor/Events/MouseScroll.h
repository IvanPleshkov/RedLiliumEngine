#pragma once

#include <Scene/Component.h>
#include <Editor/Event.h>
#include "../UserInput.h"

namespace RED_LILIUM_NAMESPACE
{

class MouseScroll : public Event
{
public:
	RED_LILIUM_CLASS(MouseScroll, Event);

	MouseScroll();
	MouseScroll(MouseState mouseState, float scrollDelta);
	~MouseScroll() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	uptr<ActionBase> TriggerAction(ptr<Editor> editor) override;

	MouseState GetMouseState() const;
	float GetScrollDelta() const;

private:
	MouseState m_mouseState;
	float m_scrollDelta;
};

} // namespace RED_LILIUM_NAMESPACE
