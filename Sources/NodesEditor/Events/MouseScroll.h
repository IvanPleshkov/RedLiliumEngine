#pragma once

#include <Scene/Component.h>
#include <Editor/Event.h>
#include <Core/InputEvent.h>

namespace RED_LILIUM_NAMESPACE
{

class MouseScroll : public Event
{
public:
	RED_LILIUM_CLASS(MouseScroll, Event);

	MouseScroll();
	MouseScroll(const InputState& inputState, float scrollDelta);
	~MouseScroll() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	uptr<ActionBase> TriggerAction(ptr<Editor> editor) override;

	const InputState& GetMouseState() const;
	float GetScrollDelta() const;

private:
	InputState m_inputState;
	float m_scrollDelta;
};

} // namespace RED_LILIUM_NAMESPACE
