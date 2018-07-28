#pragma once

#include "Core/Common.h"
#include "../Widget.h"
#include "ClickableWidget.h"
#include "../Internal/TextWidgetsHelper.h"

namespace RED_LILIUM_NAMESPACE
{

class TextField : public ClickableWidget
{
public:
	TextField();
	~TextField() override {}

	ptr<TextField> SetText(const std::string& text);
	ptr<TextField> SetColor(vec4 color);
	ptr<TextField> SetFontSize(float size);
	ptr<TextField> SetFontFamily(FontFamily fontFamily);
	ptr<TextField> SetHorizontalAlignment(HorizontalAlignment alignment);
	ptr<TextField> SetVerticalAlignment(VerticalAlignment alignment);
	ptr<TextField> SetFontSettings(const FontSettings& fontSettings);
	ptr<TextField> SetTopPadding(float topPadding);
	ptr<TextField> SetBottomPadding(float bottomPadding);
	ptr<TextField> SetLeftPadding(float leftPadding);
	ptr<TextField> SetRightPadding(float rightPadding);

protected:
	virtual void DrawCursor();

	void Draw() override;
	void UpdateDesiredSize() override;

private:
	uptr<TextWidgetsHelper> m_textWidgetsHelper;
};

} // namespace RED_LILIUM_NAMESPACE
