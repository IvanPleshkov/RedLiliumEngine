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
	m_textField->SetFontSettings(style->m_h2);

	m_textField->SetText(m_text);
}

void TextBox::UpdateDesiredSize()
{
	m_desiredSize = { 100, 24 };
}

void TextBox::Draw()
{
	const vec2 position = GetPosition();
	const vec2 size = GetSize();
	NVGcontextPtr nvg = GetNvgContext();

	const float borderWidth = 2.0f;
	const float borderRadius = 4.0f;

	nvgBeginPath(nvg);
	nvgRoundedRect(nvg, 
		position.x + borderWidth, position.y + borderWidth, 
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
