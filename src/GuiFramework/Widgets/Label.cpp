#include "pch.h"
#include "Label.h"

using namespace RED_LILIUM_NAMESPACE;

ptr<Label> Label::SetText(const std::string& text)
{
	m_text = text;
	return this;
}

ptr<Label> Label::SetColor(vec4 color)
{
	m_color = color;
	return this;
}

ptr<Label> Label::SetFontSize(float size)
{
	m_fontSize = size;
	return this;
}

ptr<Label> Label::SetFontFamily(FontFamily fontFamily)
{
	m_fontFamily = fontFamily;
	return this;
}

ptr<Label> Label::SetHorizontalAlignment(HorizontalAlignment alignment)
{
	m_horizontalAlignment = alignment;
	return this;
}

ptr<Label> Label::SetVerticalAlignment(VerticalAlignment alignment)
{
	m_verticalAlignment = alignment;
	return this;
}

ptr<Label> Label::SetTopPadding(float topMargin)
{
	m_topPadding = topMargin;
	return this;
}

ptr<Label> Label::SetBottomPadding(float bottomMargin)
{
	m_bottomPadding = bottomMargin;
	return this;
}

ptr<Label> Label::SetLeftPadding(float leftMargin)
{
	m_leftPadding = leftMargin;
	return this;
}

ptr<Label> Label::SetRightPadding(float rightMargin)
{
	m_rightPadding = rightMargin;
	return this;
}

ptr<Label> Label::SetFontSettings(const FontSettings& fontSettings)
{
	m_color = fontSettings.color;
	m_fontSize = fontSettings.fontSize;
	m_fontFamily = fontSettings.fontFamily;
	return this;
}

void Label::SetTextNvgParameters()
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

	nvgFontSize(GetNvgContext(), m_fontSize);
	nvgFontFaceId(GetNvgContext(), m_fontFamily);
	nvgFillColor(GetNvgContext(), nvgRGBAf(m_color.x, m_color.y, m_color.z, m_color.a));
	nvgTextAlign(GetNvgContext(), horizontalAlignment | verticalAlignment);
}

void Label::Draw()
{
	SetTextNvgParameters();

	vec2 thisSize = {
		m_size.x - m_leftPadding - m_rightPadding,
		m_size.y - m_topPadding - m_bottomPadding };

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
		GetNvgContext(), x, y,
		m_text.c_str(),
		m_text.c_str() + m_text.size());
}

void Label::UpdateDesiredSize()
{
	float bounds[4] = { -1.0f, -1.0f, -1.0f, -1.0f };

	SetTextNvgParameters();
	nvgTextBounds(
		GetNvgContext(),
		0.0f,
		0.0f,
		m_text.c_str(),
		m_text.c_str() + m_text.size(),
		bounds);

	m_desiredSize = {
		bounds[2] - bounds[0] + m_leftPadding + m_rightPadding,
		bounds[3] - bounds[1] + m_topPadding + m_bottomPadding
	};
}
