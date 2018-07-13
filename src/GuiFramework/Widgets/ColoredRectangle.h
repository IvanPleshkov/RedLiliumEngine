#pragma once

#include "core/common.h"
#include "GuiFramework/Widget.h"

namespace RED_LILIUM_NAMESPACE
{

class ColoredRectangle : public Widget
{
public:
	ptr<ColoredRectangle> SetColor(vec4 color);
	ptr<ColoredRectangle> SetMinimumSize(vec2 minimumSize);
	ptr<ColoredRectangle> SetDesiredSize(vec2 desiredSize);

protected:
	void UpdateDesiredSize() override;
	void UpdateMinimumSize() override;
	void Draw() override;

private:
	vec4 m_color = { 1, 1, 1, 1 };
	vec2 m_rectMinimumSize = { 0, 0 };
	vec2 m_rectDesiredSize = { 100, 100 };
};

}  // namespace RED_LILIUM_NAMESPACE
