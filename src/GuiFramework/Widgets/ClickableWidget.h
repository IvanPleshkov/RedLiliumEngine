#pragma once

#include "core/common.h"
#include "GuiFramework/Widget.h"

namespace RED_LILIUM_NAMESPACE
{

class ClickableWidget : public Widget
{
public:
	~ClickableWidget() override {}

	bool IsClicked(MouseKey mouseKey = MouseKey::Left) const;
	bool IsClicked(Flags<MouseKey> mouseKey) const;

	bool IsDoubleClicked(MouseKey mouseKey = MouseKey::Left) const;
	bool IsDoubleClicked(Flags<MouseKey> mouseKey) const;

	bool IsTripleClicked(MouseKey mouseKey = MouseKey::Left) const;
	bool IsTripleClicked(Flags<MouseKey> mouseKey) const;

	bool IsPressed(MouseKey mouseKey = MouseKey::Left) const;
	bool IsPressed(Flags<MouseKey> mouseKey) const;

	bool CanDoubleClicked(bool value) const;
	bool CanTripleClicked(bool value) const;

	bool CanFocusing() const override;

protected:
	virtual void OnClick(MouseKey mouseKey) {}
	virtual void OnDoubleClick(MouseKey mouseKey) {}
	virtual void OnTripleClick(MouseKey mouseKey) {}

	bool HandleKeyEvent(const KeyEvent& keyEvent) override;
	bool HandleMouseEvent(const MouseEvent& mouseEvent) override;

	void Update() override
	{
		m_isClicked = false;
		m_isDoubleClicked = false;
		m_isTripleClicked = false;
	}

	void SetDoubleClicked(bool value);
	void SetTripleClicked(bool value);

private:
	bool m_canDoubleClicked = false;
	bool m_canTripleClicked = false;

	bool m_isClicked = false;
	bool m_isDoubleClicked = false;
	bool m_isTripleClicked = false;

	MouseKey m_lastMouseKey = MouseKey::None;
	Time m_lastClickTime = 0;
	u32 m_clicksCount = 0;
};

} // namespace RED_LILIUM_NAMESPACE
