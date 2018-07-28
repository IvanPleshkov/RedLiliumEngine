#include "pch.h"
#include "TextBox.h"
#include "DefaultWidgetsSettings.h"

using namespace RED_LILIUM_NAMESPACE;

TextBox::TextBox()
{}

ptr<TextBox> TextBox::SetText(const std::string& text)
{
	m_text = text;
	return this;
}

void TextBox::CreateFirstChilds()
{
	m_textField = Add<TextField>();
}

void TextBox::Tick()
{
	ptr<Style> style = GetStyle();
	m_textField
		->SetFontSettings(style->m_h2)
		->SetText(m_text)
		->SetLeftPadding(DefaultWidgetsSettings::GetBorderWidth() + DefaultWidgetsSettings::GetBorderRadius())
		->SetRightPadding(DefaultWidgetsSettings::GetBorderWidth() + DefaultWidgetsSettings::GetBorderRadius());
}

void TextBox::UpdateDesiredSize()
{
	m_desiredSize = { 150, 28 };
}

void TextBox::Draw()
{
	const vec2 size = GetSize();
	NVGcontextPtr nvg = GetNvgContext();

	const float borderWidth = DefaultWidgetsSettings::GetBorderWidth();
	const float borderRadius = DefaultWidgetsSettings::GetBorderRadius();

	nvgBeginPath(nvg);
	nvgRoundedRect(nvg, 
		borderWidth, borderWidth, 
		size.x - 2 * borderWidth, size.y - 2 * borderWidth, 
		borderRadius);
	if (m_textField->IsHovered() || m_textField->IsFocused())
	{
		nvgFillColor(nvg, DefaultWidgetsSettings::GetEditableTextBackgroundHoveredColor());
	}
	else
	{
		nvgFillColor(nvg, DefaultWidgetsSettings::GetEditableTextBackgroundColor());
	}
	nvgFill(nvg);

	if (m_textField->IsFocused())
	{
		nvgStrokeColor(nvg, DefaultWidgetsSettings::GetEditableTextBorderFocusedColor());
	}
	else
	{
		nvgStrokeColor(nvg, DefaultWidgetsSettings::GetEditableTextBorderColor());
	}
	nvgStrokeWidth(nvg, borderWidth);
	nvgStroke(nvg);
}
