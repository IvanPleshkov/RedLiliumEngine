#include "pch.h"
#include "TextBox.h"
#include "DefaultWidgetsSettings.h"

using namespace RED_LILIUM_NAMESPACE;

TextBox::TextBox()
{}

ptr<TextBox> TextBox::SetText(const std::string& text)
{
	return this;
}

void TextBox::CreateFirstChilds()
{
	m_textField = Add<TextField>();
}

void TextBox::Tick()
{
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

	nvgBeginPath(nvg);
	nvgRoundedRect(nvg, position.x, position.y, size.x, size.y, 4);
	nvgFillColor(nvg, DefaultWidgetsSettings::GetEditableTextBackgroundColor());
	nvgFill(nvg);

	if (m_textField->IsFocused())
	{
		nvgStrokeColor(nvg, DefaultWidgetsSettings::GetEditableTextBorderFocusedColor());
	}
	else
	{
		nvgStrokeColor(nvg, DefaultWidgetsSettings::GetEditableTextBorderColor());
	}
	nvgStrokeWidth(nvg, 2.0f);
	nvgStroke(nvg);
}
