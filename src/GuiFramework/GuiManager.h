#pragma once

#include "core/common.h"
#include "Widget.h"
#include "Panel.h"

namespace RED_LILIUM_NAMESPACE
{

class GuiManager : public RedLiliumObject
{
public:
	GuiManager();
	~GuiManager() override { }

	ptr<Style> GetStyle();
	ptr<Panel> AddPanel();
	void RemovePanel(const ptr<Panel>& panel);

	void UpdateInputState(const MouseState& mouseState, const KeyState& keyState);

	const MouseState& GetMouseState() const;
	const KeyState& GetKeyState() const;

	ptr<Widget> GetWidgetUnderPoint(vec2 point);

private:
	friend class Widget;

	void UpdateKeyState(const KeyState& mouseState);
	void UpdateMouseState(const MouseState& mouseState);
	void UpdateHoveredWidget();
	void ChangeFocusByMouse();
	void CheckMouseKeyEvent(
		Flags<MouseKey> prevFramePressedKeys,
		MouseKey key,
		MouseEventType pressEvent,
		MouseEventType releaseEvent);

	bool HandleMouseEvent(const MouseEvent& mouseEventType);
	bool HandleKeyEvent(const KeyEvent& keyEventType);

	void SetFocusedWidget(const ptr<Widget>& focusedWidget);
	void SetHoveredWidget(const ptr<Widget>& hoveredWidget);

	std::unordered_set<uptr<Panel>> m_panels;
	ptr<Widget> m_focusedWidget;
	ptr<Widget> m_hoveredWidget;
	ptr<Widget> m_draggableWidget;
	uptr<Style> m_style;

	MouseState m_mouseState;
	KeyState m_keyState;
};

} // namespace RED_LILIUM_NAMESPACE
