#include "pch.h"
#include "ClickableWidget.h"
#include "../Panel.h"
#include "../GuiManager.h"
#include "../INativeEnviroment.h"

using namespace RED_LILIUM_NAMESPACE;

ClickableWidget::ClickableWidget()
{
	SetHovering(true);
	SetFocusing(true);
}

bool ClickableWidget::IsPressed(MouseKey mouseKey) const
{
	return m_pressedMouseKey && mouseKey == m_pressedMouseKey;
}

bool ClickableWidget::IsPressed(Flags<MouseKey> mouseKey) const
{
	return m_pressedMouseKey && mouseKey.Test(m_pressedMouseKey.value());
}

bool ClickableWidget::IsClicked(Flags<MouseKey> mouseKeys, u32 clicksCount) const
{
	if (!m_lastClick)
	{
		return false;
	}

	LastClickInfo click = m_lastClick.value();
	if (!mouseKeys.Test(click.mouseKey))
	{
		return false;
	}

	if (click.clicksCount != clicksCount)
	{
		return false;
	}

	Time now = GetGuiManager()->GetTimeFromStart();
	return click.clickTime == now;
}

bool ClickableWidget::HandleKeyEvent(const KeyEvent& keyEvent)
{
	// TODO: press by enter and space
	return false;
}

bool ClickableWidget::HandleMouseEvent(const MouseEvent& mouseEvent)
{
	if (mouseEvent.IsMouseKeyPressedEvent())
	{
		if (IsHovered() && IsFocused())
		{
			m_pressedMouseKey = mouseEvent.GetEventedMouseKey();
		}
		else
		{
			m_pressedMouseKey = std::nullopt;
		}

		return m_pressedMouseKey.has_value();
	}

	if (mouseEvent.IsMouseKeyReleasedEvent())
	{
		if (!IsHovered() || !IsFocused())
		{
			m_pressedMouseKey = std::nullopt;
			return false;
		}

		if (mouseEvent.GetEventedMouseKey())
		{
			UpdateLastClick(mouseEvent.GetEventedMouseKey().value());
		}
		else
		{
			m_lastClick = std::nullopt;
		}

		m_pressedMouseKey = std::nullopt;

		if (IsClicked())
		{
			OnClick(m_lastClick.value().mouseKey);
		}

		if (IsDoubleClicked())
		{
			OnDoubleClick(m_lastClick.value().mouseKey);
		}

		if (IsTripleClicked())
		{
			OnTripleClick(m_lastClick.value().mouseKey);
		}

		return true;
	}

	return false;
}

bool ClickableWidget::CanMultiplingLastClick(MouseKey releasedKey) const
{
	if (!m_lastClick || m_multipleClickingCount == 1)
	{
		return false;
	}

	LastClickInfo lastClick = m_lastClick.value();
	if (lastClick.mouseKey != releasedKey)
	{
		return false;
	}

	Time time = GetGuiManager()->GetTimeFromStart();
	Time timeFromLastClick = time - lastClick.clickTime;

	ptr<const INativeEnvironment> nativeEnvironment = GetGuiManager()->GetNativeEnvironment();
	Time doubleClickingTime = nativeEnvironment->GetDoubleClickingTime();

	if (timeFromLastClick > doubleClickingTime)
	{
		return false;
	}

	if (lastClick.clicksCount >= m_multipleClickingCount)
	{
		return false;
	}

	return true;
}

void ClickableWidget::UpdateLastClick(MouseKey releasedKey)
{
	if (releasedKey != m_pressedMouseKey)
	{
		m_lastClick = std::nullopt;
		return;
	}

	LastClickInfo click;
	click.mouseKey = releasedKey;
	click.clickTime = GetGuiManager()->GetTimeFromStart();
	click.clicksCount = 1;

	if (CanMultiplingLastClick(releasedKey))
	{
		click.clicksCount = m_lastClick.value().clicksCount + 1;
	}

	m_lastClick = click;
}

bool ClickableWidget::IsClicked(MouseKey mouseKey) const
{
	return IsClicked(Flags<MouseKey>(mouseKey), 1);
}

bool ClickableWidget::IsClicked(Flags<MouseKey> mouseKeys) const
{
	return IsClicked(mouseKeys, 1);
}

bool ClickableWidget::IsDoubleClicked(MouseKey mouseKey) const
{
	return IsClicked(Flags<MouseKey>(mouseKey), 2);
}

bool ClickableWidget::IsDoubleClicked(Flags<MouseKey> mouseKeys) const
{
	return IsClicked(mouseKeys, 2);
}

bool ClickableWidget::IsTripleClicked(MouseKey mouseKey) const
{
	return IsClicked(Flags<MouseKey>(mouseKey), 3);
}

bool ClickableWidget::IsTripleClicked(Flags<MouseKey> mouseKeys) const
{
	return IsClicked(mouseKeys, 3);
}

bool ClickableWidget::CanDoubleClicked() const
{
	return m_multipleClickingCount >= 2;
}

bool ClickableWidget::CanTripleClicked() const
{
	return m_multipleClickingCount >= 3;
}

void ClickableWidget::SetDoubleClicked(bool value)
{
	if (value)
	{
		m_multipleClickingCount = std::max(m_multipleClickingCount, u32(2));
	}
	else
	{
		m_multipleClickingCount = 1;
	}
}

void ClickableWidget::SetTripleClicked(bool value)
{
	if (value)
	{
		m_multipleClickingCount = 3;
	}
	else
	{
		m_multipleClickingCount = std::min(m_multipleClickingCount, u32(2));
	}
}
