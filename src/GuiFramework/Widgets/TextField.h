#pragma once

#include "Core/Common.h"
#include "../Widget.h"
#include "ClickableWidget.h"

namespace RED_LILIUM_NAMESPACE
{

class TextField : public ClickableWidget
{
public:
	TextField() {}
	~TextField() override {}

	ptr<TextField> SetText(const std::string& text);
	ptr<TextField> SetColor(vec4 color);
	ptr<TextField> SetFontSize(float size);
	ptr<TextField> SetFontFamily(FontFamily fontFamily);
	ptr<TextField> SetHorizontalAlignment(HorizontalAlignment alignment);
	ptr<TextField> SetVerticalAlignment(VerticalAlignment alignment);
	ptr<TextField> SetFontSettings(const FontSettings& fontSettings);
	ptr<TextField> SetTopPadding(float topMargin);
	ptr<TextField> SetBottomPadding(float bottomMargin);
	ptr<TextField> SetLeftPadding(float leftMargin);
	ptr<TextField> SetRightPadding(float rightMargin);

protected:
	virtual void DrawCursor();

	void Draw() override;
	void UpdateDesiredSize() override;

private:
	void SetTextNvgParameters();

	float m_topPadding = 0;
	float m_bottomPadding = 0;
	float m_leftPadding = 0;
	float m_rightPadding = 0;

	std::string m_text = "";
	std::string m_displayedText = "";
	std::optional<u32> m_cursorPosition;

	vec4 m_color = { 1, 1, 1, 1 };
	float m_fontSize = 14;
	FontFamily m_fontFamily = -1;

	HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
	VerticalAlignment m_verticalAlignment = VerticalAlignment::Center;
};

} // namespace RED_LILIUM_NAMESPACE
