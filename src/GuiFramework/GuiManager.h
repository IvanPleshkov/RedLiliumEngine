#pragma once

#include "Common.h"
#include "GuiRecorder.h"

namespace RED_LILIUM_NAMESPACE
{

class GuiManager : public RedLiliumObject
{
public:
	GuiManager(ptr<INativeEnvironment> nativeEnvironment, GuiRecordingMode recordingData = GuiRecordingMode::NoRecording);
	~GuiManager() override { }

	ptr<Style> GetStyle();
	ptr<Panel> AddPanel();
	void RemovePanel(ptr<Panel> panel);

	void BeginFrame(const Time& timeFromStart, const MouseState& mouseState, const KeyState& keyState);
	void EndFrame();

	void Draw();

	const MouseState& GetMouseState() const;
	const KeyState& GetKeyState() const;
	Time GetTimeFromStart() const;
	ptr<INativeEnvironment> GetNativeEnvironment();
	ptr<const INativeEnvironment> GetNativeEnvironment() const;

	ptr<Widget> GetWidgetUnderPoint(vec2 point);

	void LogMessage(const std::string& message);

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

	void SetFocusedWidget(ptr<Widget> focusedWidget);
	void SetHoveredWidget(ptr<Widget> hoveredWidget);

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

	uptr<GuiRecorder> m_recorder;
};

} // namespace RED_LILIUM_NAMESPACE
