#pragma once

#include <core/common.h>
#include <GuiFramework/GuiFramework.h>

namespace RED_LILIUM_NAMESPACE
{
namespace DefaultWidgetsSettings
{

inline NVGcolor GetFontColor()
{
	return nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f);
}

inline NVGcolor GetNonActiveFontColor()
{
	return nvgRGBAf(0.8f, 0.8f, 0.8f, 1.0f);
}

inline NVGcolor GetEditableTextBackgroundColor()
{
	return nvgRGBAf(0.3f, 0.3f, 0.3f, 1.0f);
}

inline NVGcolor GetEditableTextBorderColor()
{
	return nvgRGBAf(0.4f, 0.1f, 0.1f, 1.0f);
}

inline NVGcolor GetEditableTextBorderFocusedColor()
{
	return nvgRGBAf(0.6f, 0.2f, 0.2f, 1.0f);
}


}
} // namespace RED_LILIUM_NAMESPACE
