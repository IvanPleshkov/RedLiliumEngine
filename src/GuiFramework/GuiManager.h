#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class GuiManager : public RedLiliumObject
{
public:
	GuiManager(ptr<INativeEnvironment> nativeEnvironment);
	~GuiManager() override { }

	ptr<Style> GetStyle();
	ptr<Panel> AddPanel();
	void RemovePanel(const ptr<Panel>& panel);

	void BeginFrame(const Time& timeFromStart, const MouseState& mouseState, const KeyState& keyState);
	void EndFrame();

	void Draw();

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

private:
	ptr<INativeEnvironment> m_nativeEnvironment;
	std::unordered_set<uptr<Panel>> m_panels;
	ptr<Widget> m_focusedWidget;
	ptr<Widget> m_hoveredWidget;
	ptr<Widget> m_draggableWidget;
	uptr<Style> m_style;

	MouseState m_mouseState;
	KeyState m_keyState;
	Time m_timeFromStart;
};

} // namespace RED_LILIUM_NAMESPACE
