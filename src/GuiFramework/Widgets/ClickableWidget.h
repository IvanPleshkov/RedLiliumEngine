#pragma once

#include "core/common.h"
#include "GuiFramework/Widget.h"

namespace RED_LILIUM_NAMESPACE
{

class ClickableWidget : public Widget
{
public:
	bool CanFocusing() const override { return true; }

	bool IsPressed() const;

	bool IsClicked() const { return m_isClicked; }

protected:
	virtual void OnClick() {}
	virtual void OnPress() {}

	bool HandleKeyEvent(const KeyEvent& keyEvent) override
	{
		// TODO: press by enter and space
		return false;
	}

	bool HandleMouseEvent(const MouseEvent& mouseEvent) override
	{
		if (mouseEvent.eventType == MouseEventType::LeftReleased && IsHovered(true) && IsFocused())
		{
			m_isClicked = true;
			OnClick();
			return true;
		}

		return false;
	}

	void Update() override
	{
		m_isClicked = false;
	}

	void Draw() override
	{
		vec2 position = GetPosition();
		vec2 size = GetSize();
		NVGcolor color = nvgRGBAf(0, 1, 0, 1);

		if (IsHovered())
		{
			color = nvgRGBAf(1, 0, 0, 1);
		}

		if (IsPressed())
		{
			color = nvgRGBAf(0, 0, 1, 1);
		}

		nvgBeginPath(GetNvgContext());
		nvgRect(GetNvgContext(), position.x, position.y, size.x, size.y);
		nvgFillColor(GetNvgContext(), color);
		nvgFill(GetNvgContext());
	}

private:
	bool m_isClicked = false;
};

}  // namespace RED_LILIUM_NAMESPACE
