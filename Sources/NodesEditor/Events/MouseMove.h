#pragma once

#include <Scene/Component.h>
#include <Editor/Event.h>
#include <Core/InputEvent.h>

namespace RED_LILIUM_NAMESPACE
{

class MouseMove : public Event
{
public:
	RED_LILIUM_CLASS(MouseMove, Event);

	MouseMove();
	MouseMove(const InputState& inputState, vec2 prevMousePos);
	~MouseMove() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
	InputState m_inputState;
	vec2 m_prevMousePos;
};

} // namespace RED_LILIUM_NAMESPACE
