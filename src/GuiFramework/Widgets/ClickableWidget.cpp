#include "pch.h"
#include "ClickableWidget.h"

using namespace RED_LILIUM_NAMESPACE;

bool ClickableWidget::IsPressed() const
{
	return IsHovered(true) && IsFocused() && GetMouseState().pressedKeys.Test(MouseKey::Left);
}
