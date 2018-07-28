#pragma once

#include <core/common.h>
#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{
namespace DefaultWidgetsSettings
{

inline NVGcolor GetFontColor()
{
	return nvgRGBA(0xff, 0xff, 0xff, 0xff);
}

inline NVGcolor GetNonActiveFontColor()
{
	return nvgRGBA(0xa0, 0xa0, 0xa0, 0xff);
}

inline NVGcolor GetEditableTextBackgroundColor()
{
	return nvgRGBA(0x20, 0x20, 0x20, 0xff);
}

inline NVGcolor GetEditableTextBackgroundHoveredColor()
{
	return nvgRGBA(0x30, 0x30, 0x30, 0xff);
}

inline NVGcolor GetEditableTextBorderColor()
{
	return nvgRGBA(0x40, 0x40, 0x40, 0xff);
}

inline NVGcolor GetEditableTextBorderFocusedColor()
{
	return nvgRGBA(0x20, 0x50, 0x90, 0xff);
}

inline float GetBorderWidth()
{
	return 2.0f;
}

inline float GetBorderRadius()
{
	return 4.0f;
}

}
} // namespace RED_LILIUM_NAMESPACE
