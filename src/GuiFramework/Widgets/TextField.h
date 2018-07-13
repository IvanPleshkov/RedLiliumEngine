#pragma once

#include "core/common.h"
#include "GuiFramework/Widget.h"

namespace RED_LILIUM_NAMESPACE
{

class Label : public Widget
{
public:
	ptr<Label> SetText(const std::string& text);
	ptr<Label> SetColor(vec4 color);
	ptr<Label> SetFontSize(float size);
	ptr<Label> SetFontFamily(FontFamily fontFamily);
	ptr<Label> SetHorizontalAlignment(HorizontalAlignment alignment);
	ptr<Label> SetVerticalAlignment(VerticalAlignment alignment);
	ptr<Label> SetFontSettings(const FontSettings& fontSettings);
	ptr<Label> SetTopPadding(float topMargin);
	ptr<Label> SetBottomPadding(float bottomMargin);
	ptr<Label> SetLeftPadding(float leftMargin);
	ptr<Label> SetRightPadding(float rightMargin);

protected:
	void Draw() override;
	void UpdateDesiredSize() override;

private:
	void SetTextNvgParameters();

	float m_topPadding = 0;
	float m_bottomPadding = 0;
	float m_leftPadding = 0;
	float m_rightPadding = 0;
	std::string m_text = "";
	vec4 m_color = { 1, 1, 1, 1 };
	float m_fontSize = 14;
	FontFamily m_fontFamily = -1;
	HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Center;
	VerticalAlignment m_verticalAlignment = VerticalAlignment::Center;
};

} // namespace RED_LILIUM_NAMESPACE
