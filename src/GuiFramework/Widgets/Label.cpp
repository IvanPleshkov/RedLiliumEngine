#include "pch.h"
#include "Label.h"

using namespace RED_LILIUM_NAMESPACE;

Label::Label()
	: m_textWidgetsHelper(std::move(umake<TextWidgetsHelper>()))
	, m_disaplyedText()
{}

ptr<Label> Label::SetText(const std::string& text)
{
	m_disaplyedText = text;
	return this;
}

ptr<Label> Label::SetColor(vec4 color)
{
	m_textWidgetsHelper->SetColor(color);
	return this;
}

ptr<Label> Label::SetFontSize(float size)
{
	m_textWidgetsHelper->SetFontSize(size);
	return this;
}

ptr<Label> Label::SetFontFamily(FontFamily fontFamily)
{
	m_textWidgetsHelper->SetFontFamily(fontFamily);
	return this;
}

ptr<Label> Label::SetHorizontalAlignment(HorizontalAlignment alignment)
{
	m_textWidgetsHelper->SetHorizontalAlignment(alignment);
	return this;
}

ptr<Label> Label::SetVerticalAlignment(VerticalAlignment alignment)
{
	m_textWidgetsHelper->SetVerticalAlignment(alignment);
	return this;
}

ptr<Label> Label::SetTopPadding(float topPadding)
{
	m_textWidgetsHelper->SetTopPadding(topPadding);
	return this;
}

ptr<Label> Label::SetBottomPadding(float bottomPadding)
{
	m_textWidgetsHelper->SetBottomPadding(bottomPadding);
	return this;
}

ptr<Label> Label::SetLeftPadding(float leftPadding)
{
	m_textWidgetsHelper->SetLeftPadding(leftPadding);
	return this;
}

ptr<Label> Label::SetRightPadding(float rightPadding)
{
	m_textWidgetsHelper->SetRightPadding(rightPadding);
	return this;
}

ptr<Label> Label::SetFontSettings(const FontSettings& fontSettings)
{
	m_textWidgetsHelper->SetFontSettings(fontSettings);
	return this;
}

void Label::Draw()
{
	m_textWidgetsHelper->Draw(GetNvgContext(), m_disaplyedText, { 0, 0 }, GetSize());
}

void Label::UpdateDesiredSize()
{
	m_desiredSize = m_textWidgetsHelper->GetDisplayedTextSize(GetNvgContext(), m_disaplyedText);
}
