#pragma once

#include "Core/Common.h"
#include "../Common.h"

namespace RED_LILIUM_NAMESPACE
{

class TextWidgetsHelper : public RedLiliumObject
{
public:
	TextWidgetsHelper();
	~TextWidgetsHelper() override {}

	void Draw(NVGcontextPtr nvg, vec2 textBoxPosition, vec2 textBoxSize);

	void SetFontSettings(const FontSettings& fontSettings);
	vec2 GetDisplayedTextSize(NVGcontextPtr nvg) const;

	void SetDisplayedText(const std::string& text);
	void SetColor(vec4 color);
	void SetFontSize(float size);
	void SetFontFamily(FontFamily fontFamily);
	void SetHorizontalAlignment(HorizontalAlignment alignment);
	void SetVerticalAlignment(VerticalAlignment alignment);
	void SetTopPadding(float topMargin);
	void SetBottomPadding(float bottomMargin);
	void SetLeftPadding(float leftMargin);
	void SetRightPadding(float rightMargin);

	const std::string& GetDisplayedText() const;
	vec4 GetColor() const;
	float GetFontSize() const;
	FontFamily GetFontFamily() const;
	HorizontalAlignment GetHorizontalAlignment() const;
	VerticalAlignment GetVerticalAlignment() const;
	float GetTopPadding() const;
	float GetBottomPadding() const;
	float GetLeftPadding() const;
	float GetRightPadding() const;

private:
	void SetTextNvgParameters(NVGcontextPtr nvg) const;

	std::string m_displayedText;

	float m_topPadding;
	float m_bottomPadding;
	float m_leftPadding;
	float m_rightPadding;

	vec4 m_color;
	float m_fontSize;
	FontFamily m_fontFamily;

	HorizontalAlignment m_horizontalAlignment;
	VerticalAlignment m_verticalAlignment;
};

} // namespace RED_LILIUM_NAMESPACE
