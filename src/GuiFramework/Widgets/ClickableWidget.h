#pragma once

#include "core/common.h"
#include "GuiFramework/Widget.h"

namespace RED_LILIUM_NAMESPACE
{

class ClickableWidget : public Widget
{
public:
	ClickableWidget();
	~ClickableWidget() override {}

	bool IsClicked(MouseKey mouseKey = MouseKey::Left) const;
	bool IsClicked(Flags<MouseKey> mouseKeys) const;

	bool IsDoubleClicked(MouseKey mouseKey = MouseKey::Left) const;
	bool IsDoubleClicked(Flags<MouseKey> mouseKeys) const;

	bool IsTripleClicked(MouseKey mouseKey = MouseKey::Left) const;
	bool IsTripleClicked(Flags<MouseKey> mouseKeys) const;

	bool IsPressed(MouseKey mouseKey = MouseKey::Left) const;
	bool IsPressed(Flags<MouseKey> mouseKeys) const;

	bool IsEntered() const;

	bool CanDoubleClicked() const;
	bool CanTripleClicked() const;

protected:
	bool HandleKeyEvent(const KeyEvent& keyEvent) override;
	bool HandleMouseEvent(const MouseEvent& mouseEvent) override;

	virtual void OnPress(MouseKey mouseKey) {}
	virtual void OnClick(MouseKey mouseKey) {}
	virtual void OnDoubleClick(MouseKey mouseKey) {}
	virtual void OnTripleClick(MouseKey mouseKey) {}
	virtual void OnEnter(Key key) {}

	void SetDoubleClicked(bool value);
	void SetTripleClicked(bool value);

private:
	struct LastClickInfo
	{
		MouseKey mouseKey = MouseKey::Left;
		Time clickTime = 0;
		u32 clicksCount = 0;
	};

	bool IsClicked(Flags<MouseKey> mouseKeys, u32 clicksCount) const;
	bool CanMultiplingLastClick(MouseKey releasedKey) const;
	void UpdateLastClick(MouseKey releasedKey);

	u32 m_multipleClickingCount = 1;

	std::optional<MouseKey> m_pressedMouseKey = std::nullopt;
	std::optional<LastClickInfo> m_lastClick = std::nullopt;
};

} // namespace RED_LILIUM_NAMESPACE
