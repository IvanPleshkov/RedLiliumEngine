#include "pch.h"
#include "TextWidgetsHelper.h"

using namespace RED_LILIUM_NAMESPACE;

TextWidgetsHelper::TextWidgetsHelper()
	: m_topPadding(0)
	, m_bottomPadding(0)
	, m_leftPadding(0)
	, m_rightPadding(0)
	, m_textOffset(0)
	, m_color( 1, 1, 1, 1 )
	, m_fontSize(14)
	, m_fontFamily(-1)
	, m_horizontalAlignment(HorizontalAlignment::Left)
	, m_verticalAlignment(VerticalAlignment::Center)
{}

void TextWidgetsHelper::SetColor(vec4 color)
{
	m_color = color;
}

void TextWidgetsHelper::SetFontSize(float size)
{
	m_fontSize = size;
}

void TextWidgetsHelper::SetFontFamily(FontFamily fontFamily)
{
	m_fontFamily = fontFamily;
}

void TextWidgetsHelper::SetHorizontalAlignment(HorizontalAlignment alignment)
{
	m_horizontalAlignment = alignment;
}

void TextWidgetsHelper::SetVerticalAlignment(VerticalAlignment alignment)
{
	m_verticalAlignment = alignment;
}

void TextWidgetsHelper::SetTopPadding(float topPadding)
{
	m_topPadding = topPadding;
}

void TextWidgetsHelper::SetBottomPadding(float bottomPadding)
{
	m_bottomPadding = bottomPadding;
}

void TextWidgetsHelper::SetLeftPadding(float leftPadding)
{
	m_leftPadding = leftPadding;
}

void TextWidgetsHelper::SetRightPadding(float rightPadding)
{
	m_rightPadding = rightPadding;
}

vec4 TextWidgetsHelper::GetColor() const
{
	return m_color;
}

float TextWidgetsHelper::GetFontSize() const
{
	return m_fontSize;
}

FontFamily TextWidgetsHelper::GetFontFamily() const
{
	return m_fontFamily;
}

HorizontalAlignment TextWidgetsHelper::GetHorizontalAlignment() const
{
	return m_horizontalAlignment;
}

VerticalAlignment TextWidgetsHelper::GetVerticalAlignment() const
{
	return m_verticalAlignment;
}

float TextWidgetsHelper::GetTopPadding() const
{
	return m_topPadding;
}

float TextWidgetsHelper::GetBottomPadding() const
{
	return m_bottomPadding;
}

float TextWidgetsHelper::GetLeftPadding() const
{
	return m_leftPadding;
}

float TextWidgetsHelper::GetRightPadding() const
{
	return m_rightPadding;
}

void TextWidgetsHelper::SetFontSettings(const FontSettings& fontSettings)
{
	m_color = fontSettings.color;
	m_fontSize = fontSettings.fontSize;
	m_fontFamily = fontSettings.fontFamily;
}

vec2 TextWidgetsHelper::GetDisplayedTextSize(NVGcontextPtr nvg, const std::string_view& displayedText) const
{
	float bounds[4] = { -1.0f, -1.0f, -1.0f, -1.0f };

	SetTextNvgParameters(nvg);
	nvgTextBounds(
		nvg,
		0.0f,
		0.0f,
		displayedText.data(),
		displayedText.data() + displayedText.size(),
		bounds);

	return {
		bounds[2] - bounds[0] + m_leftPadding + m_rightPadding,
		bounds[3] - bounds[1] + m_topPadding + m_bottomPadding
	};
}

void TextWidgetsHelper::GetDisplayedTextGlyphsPosition(
	NVGcontextPtr nvg,
	const std::string_view& displayedText,
	vec2 textBoxPosition,
	std::vector<NVGglyphPosition>& glyphsPosition) const
{
	SetTextNvgParameters(nvg);

	glyphsPosition.resize(displayedText.size());
	nvgTextGlyphPositions(
		nvg,
		textBoxPosition.x,
		textBoxPosition.y,
		displayedText.data(),
		displayedText.data() + displayedText.size(),
		glyphsPosition.data(),
		displayedText.size());
}

void TextWidgetsHelper::SetTextNvgParameters(NVGcontextPtr nvg) const
{
	int horizontalAlignment, verticalAlignment;

	switch (m_horizontalAlignment)
	{
	case HorizontalAlignment::Center:
		horizontalAlignment = NVG_ALIGN_CENTER;
		break;
	case HorizontalAlignment::Left:
		horizontalAlignment = NVG_ALIGN_LEFT;
		break;
	case HorizontalAlignment::Right:
		horizontalAlignment = NVG_ALIGN_RIGHT;
		break;
	case HorizontalAlignment::Fill:
		horizontalAlignment = NVG_ALIGN_CENTER;
		break;
	default:
		horizontalAlignment = NVG_ALIGN_CENTER;
		break;
	}

	switch (m_verticalAlignment)
	{
	case VerticalAlignment::Center:
		verticalAlignment = NVG_ALIGN_MIDDLE;
		break;
	case VerticalAlignment::Top:
		verticalAlignment = NVG_ALIGN_TOP;
		break;
	case VerticalAlignment::Bottom:
		verticalAlignment = NVG_ALIGN_BOTTOM;
		break;
	case VerticalAlignment::Fill:
		verticalAlignment = NVG_ALIGN_BASELINE;
		break;
	default:
		verticalAlignment = NVG_ALIGN_BASELINE;
		break;
	}

	nvgFontSize(nvg, m_fontSize);
	nvgFontFaceId(nvg, m_fontFamily);
	nvgFillColor(nvg, nvgRGBAf(m_color.x, m_color.y, m_color.z, m_color.a));
	nvgTextAlign(nvg, horizontalAlignment | verticalAlignment);
}

void TextWidgetsHelper::Draw(NVGcontextPtr nvg, const std::string_view& displayedText, vec2 textBoxPosition, vec2 textBoxSize)
{
	SetTextNvgParameters(nvg);

	nvgIntersectScissor(nvg, 
		textBoxPosition.x + m_leftPadding, 
		textBoxPosition.y + m_topPadding, 
		textBoxSize.x - m_leftPadding - m_rightPadding, 
		textBoxSize.y - m_topPadding - m_bottomPadding);

	vec2 thisSize = {
		textBoxSize.x - m_leftPadding - m_rightPadding,
		textBoxSize.y - m_topPadding - m_bottomPadding };

	float x, y;
	switch (m_horizontalAlignment)
	{
	case HorizontalAlignment::Center:
		x = m_leftPadding + thisSize.x / 2.0f;
		break;
	case HorizontalAlignment::Left:
		x = m_leftPadding;
		break;
	case HorizontalAlignment::Right:
		x = m_leftPadding + thisSize.x;
		break;
	case HorizontalAlignment::Fill:
		x = m_leftPadding + thisSize.x / 2.0f;
		break;
	default:
		x = m_leftPadding + thisSize.x / 2.0f;
		break;
	}
	x -= m_textOffset;

	switch (m_verticalAlignment)
	{
	case VerticalAlignment::Center:
		y = m_topPadding + thisSize.y / 2.0f;
		break;
	case VerticalAlignment::Top:
		y = m_topPadding;
		break;
	case VerticalAlignment::Bottom:
		y = m_topPadding + thisSize.y;
		break;
	case VerticalAlignment::Fill:
		y = m_topPadding + thisSize.y / 2.0f;
		break;
	default:
		y = m_topPadding + thisSize.y / 2.0f;
		break;
	}

	nvgText(
		nvg, x, y,
		displayedText.data(),
		displayedText.data() + displayedText.size());
}
