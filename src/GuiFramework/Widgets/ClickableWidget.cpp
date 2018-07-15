#include "pch.h"
#include "ClickableWidget.h"

using namespace RED_LILIUM_NAMESPACE;

ClickableWidget::ClickableWidget()
{
	SetHovering(true);
	SetFocusing(true);
}

bool ClickableWidget::IsPressed(MouseKey mouseKey) const
{
	return IsHovered() && IsFocused() && GetMouseState().pressedKeys.Test(mouseKey);
}

bool ClickableWidget::IsPressed(Flags<MouseKey> mouseKey) const
{
	return false;
}

bool ClickableWidget::IsClicked(MouseKey mouseKey) const
{
	return m_isClicked;
}

bool ClickableWidget::IsClicked(Flags<MouseKey> mouseKey) const
{
	return m_isClicked;
}

bool ClickableWidget::HandleKeyEvent(const KeyEvent& keyEvent)
{
	// TODO: press by enter and space
	return false;
}

bool ClickableWidget::HandleMouseEvent(const MouseEvent& mouseEvent)
{
	//if (mouseEvent.eventType == MouseEventType::LeftReleased && IsHovered(true) && IsFocused())
	//{
	//	m_isClicked = true;
	//	OnClick();
	//	return true;
	//}

	return false;
}

void ClickableWidget::PostTick()
{
	m_isClicked = false;
	m_isDoubleClicked = false;
	m_isTripleClicked = false;
}

bool ClickableWidget::IsDoubleClicked(MouseKey mouseKey) const
{
	return m_isDoubleClicked;
}

bool ClickableWidget::IsDoubleClicked(Flags<MouseKey> mouseKey) const
{
	return m_isDoubleClicked;
}

bool ClickableWidget::IsTripleClicked(MouseKey mouseKey) const
{
	return m_isTripleClicked;
}

bool ClickableWidget::IsTripleClicked(Flags<MouseKey> mouseKey) const
{
	return m_isTripleClicked;
}

bool ClickableWidget::CanDoubleClicked(bool value) const
{
	return m_canDoubleClicked;
}

bool ClickableWidget::CanTripleClicked(bool value) const
{
	return m_canTripleClicked;
}

void ClickableWidget::SetDoubleClicked(bool value)
{
	m_canDoubleClicked = value;
}

void ClickableWidget::SetTripleClicked(bool value)
{
	m_canTripleClicked = value;
}
