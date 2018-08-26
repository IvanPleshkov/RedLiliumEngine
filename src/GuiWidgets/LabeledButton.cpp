#include "pch.h"
#include "LabeledButton.h"

using namespace RED_LILIUM_NAMESPACE;

ptr<LabeledButton> LabeledButton::SetText(const std::string& text)
{
	m_text = text;
	return this;
}

void LabeledButton::CreateFirstChilds()
{
	m_label = Add<Label>();
}

void LabeledButton::Tick()
{
    ptr<Style> style = GetStyle();
    m_label
        ->SetFontSettings(style->m_h2)
        ->SetText(m_text)
        ->SetHorizontalAlignment(HorizontalAlignment::Center);
}

void LabeledButton::UpdateDesiredSize()
{
	ClickableWidget::UpdateDesiredSize();
	m_desiredSize += vec2(20, 10);
}

void LabeledButton::Draw()
{
	vec2 size = GetSize();
	NVGcolor color = nvgRGBAf(0, 0.2f, 0, 1);

	if (IsHovered())
	{
		color = nvgRGBAf(0.2f, 0, 0, 1);
	}

	if (IsPressed())
	{
		color = nvgRGBAf(0, 0, 0.2f, 1);
	}

	nvgBeginPath(GetNvgContext());
	nvgRect(GetNvgContext(), 0, 0, size.x, size.y);
	nvgFillColor(GetNvgContext(), color);
	nvgFill(GetNvgContext());
}
