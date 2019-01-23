#pragma once

#include <Scene/Component.h>
#include <Editor/Event.h>
#include "../UserInput.h"

namespace RED_LILIUM_NAMESPACE
{

class MouseKeyDown : public Event
{
public:
	RED_LILIUM_CLASS(MouseKeyDown, Event);

	MouseKeyDown();
	MouseKeyDown(MouseState mouseState, MouseKey mouseKey);
	~MouseKeyDown() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
	MouseState m_mouseState;
	MouseKey m_mouseKey;
};

} // namespace RED_LILIUM_NAMESPACE
