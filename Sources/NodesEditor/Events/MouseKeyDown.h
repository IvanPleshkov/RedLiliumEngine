#pragma once

#include <Scene/Component.h>
#include <Editor/Event.h>
#include <Core/InputEvent.h>

namespace RED_LILIUM_NAMESPACE
{

class MouseKeyDown : public Event
{
public:
	RED_LILIUM_CLASS(MouseKeyDown, Event);

	MouseKeyDown();
	MouseKeyDown(const InputState& inputState, MouseKey mouseKey);
	~MouseKeyDown() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
	InputState m_inputState;
	MouseKey m_mouseKey;
};

} // namespace RED_LILIUM_NAMESPACE
