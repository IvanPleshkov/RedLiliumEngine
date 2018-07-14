#include "pch.h"
#include "GuiManager.h"
#include "Panel.h"
#include "Widget.h"

using namespace RED_LILIUM_NAMESPACE;

GuiManager::GuiManager()
	: m_focusedWidget(nullptr)
	, m_hoveredWidget(nullptr)
	, m_draggableWidget(nullptr)
{
	m_style = std::make_unique<Style>();
}

ptr<Style> GuiManager::GetStyle()
{
	return m_style.get();
}

const MouseState& GuiManager::GetMouseState() const
{
	return m_mouseState;
}

const KeyState& GuiManager::GetKeyState() const
{
	return m_keyState;
}

void GuiManager::UpdateFrame(const Time& timeFromStart, const MouseState& mouseState, const KeyState& keyState)
{
	m_timeFromStart = timeFromStart;
	UpdateKeyState(keyState);
	UpdateMouseState(mouseState);
}

void GuiManager::Draw()
{

}

void GuiManager::UpdateKeyState(const KeyState& mouseState)
{
	// TODO:
	//RED_LILIUM_ASSERT(false && "Not implemented");
}

void GuiManager::UpdateMouseState(const MouseState& mouseState)
{
	MouseState oldState = m_mouseState;
	m_mouseState = mouseState;

	// update hovered widget
	if (oldState.hoveredWindow != m_mouseState.hoveredWindow ||
		oldState.mousePosition != m_mouseState.mousePosition)
	{
		UpdateHoveredWidget();
	}

	// change focus by mouse click
	if (oldState.pressedKeys.Test(MouseKey::Left) == false &&
		m_mouseState.pressedKeys.Test(MouseKey::Left) == true)
	{
		ChangeFocusByMouse();
	}

	// mouse move event
	if (oldState.hoveredWindow != m_mouseState.hoveredWindow ||
		oldState.mousePosition != m_mouseState.mousePosition)
	{
		MouseEvent mouseMoveEvent;
		mouseMoveEvent.eventType = MouseEventType::MouseMove;
		HandleMouseEvent(mouseMoveEvent);
	}

	// mouse key events
	CheckMouseKeyEvent(oldState.pressedKeys, MouseKey::Left, MouseEventType::LeftPressed, MouseEventType::LeftReleased);
	CheckMouseKeyEvent(oldState.pressedKeys, MouseKey::Right, MouseEventType::RightPressed, MouseEventType::RightReleased);
	CheckMouseKeyEvent(oldState.pressedKeys, MouseKey::Middle, MouseEventType::MiddlePressed, MouseEventType::MiddleReleased);
}

void GuiManager::CheckMouseKeyEvent(
	Flags<MouseKey> prevFramePressedKeys,
	MouseKey mouseKey,
	MouseEventType pressEvent,
	MouseEventType releaseEvent)
{
	if (prevFramePressedKeys.Test(mouseKey) == m_mouseState.pressedKeys.Test(mouseKey))
	{
		return;
	}
	
	MouseEvent mouseEvent;
	if (m_mouseState.pressedKeys.Test(mouseKey))
	{
		mouseEvent.eventType = pressEvent;
	}
	else
	{
		mouseEvent.eventType = releaseEvent;
	}
	HandleMouseEvent(mouseEvent);
}

ptr<Panel> GuiManager::AddPanel()
{
	uptr<Panel> newPanel = std::make_unique<Panel>(this);
	ptr<Panel> result = newPanel.get();
	m_panels.insert(std::move(newPanel));
	return result;
}

void GuiManager::RemovePanel(const ptr<Panel>& panel)
{
	// TODO:
	RED_LILIUM_ASSERT(false && "Not implemented");
}

bool GuiManager::HandleKeyEvent(const KeyEvent& keyEvent)
{
	ptr<Widget> focus = m_focusedWidget;
	while (focus != nullptr)
	{
		if (focus->HandleKeyEvent(keyEvent))
		{
			return true;
		}
		focus = focus->m_parent;
	}
	return false;
}

bool GuiManager::HandleMouseEvent(const MouseEvent& mouseEvent)
{
	ptr<Widget> hover = m_hoveredWidget;

	// when left pressed of released, send event to focused widget
	if (m_mouseState.pressedKeys.Test(MouseKey::Left) ||
		mouseEvent.eventType == MouseEventType::LeftReleased)
	{
		hover = m_focusedWidget;
	}

	while (hover != nullptr)
	{
		if (hover->HandleMouseEvent(mouseEvent))
		{
			return true;
		}
		hover = hover->m_parent;
	}
	return false;
}

ptr<Widget> GuiManager::GetWidgetUnderPoint(vec2 point)
{
	// TODO: z-indexing for panels

	ptr<Widget> hoveredWidget = nullptr;
	for (auto& panel : m_panels)
	{
		hoveredWidget = panel->GetWidgetUnderPoint(point);

		if (hoveredWidget)
		{
			break;
		}
	}

	return hoveredWidget;
}

void GuiManager::UpdateHoveredWidget()
{
	ptr<Widget> widgetUnderMouse = GetWidgetUnderPoint(m_mouseState.mousePosition);

	if (m_mouseState.pressedKeys.Test(MouseKey::Left))
	{
		while (widgetUnderMouse != nullptr && widgetUnderMouse != m_focusedWidget)
		{
			widgetUnderMouse = widgetUnderMouse->m_parent;
		}
	}

	SetHoveredWidget(widgetUnderMouse);
}

void GuiManager::ChangeFocusByMouse()
{
	ptr<Widget> hoveredWidget = m_hoveredWidget;
	while (hoveredWidget != nullptr)
	{
		if (hoveredWidget->CanFocusing())
		{
			SetFocusedWidget(hoveredWidget);
			break;
		}

		hoveredWidget = hoveredWidget->m_parent;
	}
}

void GuiManager::SetFocusedWidget(const ptr<Widget>& focusedWidget)
{
	if (focusedWidget == m_focusedWidget)
	{
		return;
	}

	if (m_focusedWidget != nullptr)
	{
		m_focusedWidget->OnFocusLost();
	}

	m_focusedWidget = focusedWidget;
	if (focusedWidget != nullptr)
	{
		focusedWidget->OnFocus();
	}
}

void GuiManager::SetHoveredWidget(const ptr<Widget>& hoveredWidget)
{
	if (hoveredWidget == m_hoveredWidget)
	{
		return;
	}

	if (m_hoveredWidget != nullptr)
	{
		m_hoveredWidget->OnHoverLost();
	}

	m_hoveredWidget = hoveredWidget;
	if (hoveredWidget != nullptr)
	{
		hoveredWidget->OnHover();
	}
}
