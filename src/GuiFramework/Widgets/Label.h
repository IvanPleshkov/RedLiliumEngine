#pragma once

#include <Core/Common.h>
#include "GuiFramework/Widget.h"
#include "../Internal/TextWidgetsHelper.h"

namespace RED_LILIUM_NAMESPACE
{

class Label : public Widget
{
public:
	Label();
	~Label() override {}

	ptr<Label> SetText(const std::string& text);
	ptr<Label> SetColor(vec4 color);
	ptr<Label> SetFontSize(float size);
	ptr<Label> SetFontFamily(FontFamily fontFamily);
	ptr<Label> SetHorizontalAlignment(HorizontalAlignment alignment);
	ptr<Label> SetVerticalAlignment(VerticalAlignment alignment);
	ptr<Label> SetFontSettings(const FontSettings& fontSettings);
	ptr<Label> SetTopPadding(float topPadding);
	ptr<Label> SetBottomPadding(float bottomPadding);
	ptr<Label> SetLeftPadding(float leftPadding);
	ptr<Label> SetRightPadding(float rightPadding);

protected:
	void Draw() override;
	void UpdateDesiredSize() override;

private:
	uptr<TextWidgetsHelper> m_textWidgetsHelper;
	std::string m_disaplyedText;
};

}  // namespace RED_LILIUM_NAMESPACE
