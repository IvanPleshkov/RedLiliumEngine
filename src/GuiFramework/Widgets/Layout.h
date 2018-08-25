#pragma once

#include <Core/Common.h>
#include "GuiFramework/Widget.h"

namespace RED_LILIUM_NAMESPACE
{

class LayoutWidget : public Widget
{
protected:
	void UpdateDesiredSize() override = 0;
	void UpdateMinimumSize() override = 0;
	void Layout() override = 0;
};

} // namespace RED_LILIUM_NAMESPACE
