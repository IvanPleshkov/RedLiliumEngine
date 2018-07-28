#include "pch.h"
#include "TextField.h"

using namespace RED_LILIUM_NAMESPACE;

TextField::TextField()
	: m_textWidgetsHelper(std::move(std::make_unique<TextWidgetsHelper>()))
{}

ptr<TextField> TextField::SetText(const std::string& text)
{
	m_textWidgetsHelper->SetDisplayedText(text);
	return this;
}

ptr<TextField> TextField::SetColor(vec4 color)
{
	m_textWidgetsHelper->SetColor(color);
	return this;
}

ptr<TextField> TextField::SetFontSize(float fontSize)
{
	m_textWidgetsHelper->SetFontSize(fontSize);
	return this;
}

ptr<TextField> TextField::SetFontFamily(FontFamily fontFamily)
{
	m_textWidgetsHelper->SetFontFamily(fontFamily);
	return this;
}

ptr<TextField> TextField::SetHorizontalAlignment(HorizontalAlignment alignment)
{
	m_textWidgetsHelper->SetHorizontalAlignment(alignment);
	return this;
}

ptr<TextField> TextField::SetVerticalAlignment(VerticalAlignment alignment)
{
	m_textWidgetsHelper->SetVerticalAlignment(alignment);
	return this;
}

ptr<TextField> TextField::SetTopPadding(float topPadding)
{
	m_textWidgetsHelper->SetTopPadding(topPadding);
	return this;
}

ptr<TextField> TextField::SetBottomPadding(float bottomPadding)
{
	m_textWidgetsHelper->SetBottomPadding(bottomPadding);
	return this;
}

ptr<TextField> TextField::SetLeftPadding(float leftPadding)
{
	m_textWidgetsHelper->SetLeftPadding(leftPadding);
	return this;
}

ptr<TextField> TextField::SetRightPadding(float rightPadding)
{
	m_textWidgetsHelper->SetRightPadding(rightPadding);
	return this;
}

ptr<TextField> TextField::SetFontSettings(const FontSettings& fontSettings)
{
	m_textWidgetsHelper->SetFontSettings(fontSettings);
	return this;
}

void TextField::Draw()
{
	m_textWidgetsHelper->Draw(GetNvgContext(), { 0, 0 }, m_size);

	auto posOpt = GetLocalMousePosition();
	if (posOpt)
	{
		vec2 mousePos = posOpt.value();

		nvgBeginPath(GetNvgContext());
		nvgCircle(GetNvgContext(), mousePos.x, mousePos.y, 5);
		nvgFillColor(GetNvgContext(), nvgRGBA(0xff, 0x00, 0x00, 0xff));
		nvgFill(GetNvgContext());
	}
}

void TextField::DrawCursor()
{
}

void TextField::UpdateDesiredSize()
{
	m_desiredSize = m_textWidgetsHelper->GetDisplayedTextSize(GetNvgContext());
}
