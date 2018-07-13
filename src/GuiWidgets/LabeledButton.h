#pragma once

#include <core/common.h>
#include <GuiFramework/Widgets/ClickableWidget.h>
#include <GuiFramework/Widgets/Label.h>

namespace RED_LILIUM_NAMESPACE
{

class LabeledButton : public ClickableWidget
{
public:
	ptr<LabeledButton> SetText(const std::string& text)
	{
		m_text = text;
		return this;
	}

protected:
	void CreateFirstChilds() override
	{
		m_label = Add<Label>();
	}

	void Update() override
	{
		ptr<Style> style = GetStyle();
		m_label->SetFontSettings(style->m_h2);
		m_label->SetText(m_text);
	}

	void UpdateDesiredSize() override
	{
		ClickableWidget::UpdateDesiredSize();
		m_desiredSize += vec2( 20, 10 );
	}

	void Draw() override
	{
		vec2 position = GetPosition();
		vec2 size = GetSize();
		NVGcolor color = nvgRGBAf(0, 0.2f, 0, 1);

		if (IsHovered(true))
		{
			color = nvgRGBAf(0.2f, 0, 0, 1);
		}

		if (IsPressed())
		{
			color = nvgRGBAf(0, 0, 0.2f, 1);
		}

		nvgBeginPath(GetNvgContext());
		nvgRect(GetNvgContext(), position.x, position.y, size.x, size.y);
		nvgFillColor(GetNvgContext(), color);
		nvgFill(GetNvgContext());
	}

private:
	std::string m_text = "OK";
	ptr<Label> m_label = nullptr;
};

} // namespace RED_LILIUM_NAMESPACE
