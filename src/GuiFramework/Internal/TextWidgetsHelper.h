#pragma once

#include "Core/Common.h"
#include "../Common.h"

namespace RED_LILIUM_NAMESPACE
{

class TextWidgetsHelper : public RedLiliumObject
{
public:
	TextWidgetsHelper();
	~TextWidgetsHelper() override {}

	void Draw(NVGcontextPtr nvg, const std::string_view& displayedText, vec2 textBoxPosition, vec2 textBoxSize);

	void SetFontSettings(const FontSettings& fontSettings);
	vec2 GetDisplayedTextSize(NVGcontextPtr nvg, const std::string_view& displayedText) const;
	std::optional<u32> GetSymbolUnderPoint(
		NVGcontextPtr nvg,
		const std::string_view& displayedText,
		vec2 textBoxPosition, vec2 textBoxSize,
		vec2 pointPosition) const;

	float GetSymbolGlyphBeginPosition(
		NVGcontextPtr nvg,
		const std::string_view& displayedText,
		vec2 textBoxPosition, vec2 textBoxSize,
		u32 symbolIndex) const;
	float GetSymbolGlyphEndPosition(
		NVGcontextPtr nvg,
		const std::string_view& displayedText,
		vec2 textBoxPosition, vec2 textBoxSize,
		u32 symbolIndex) const;
	float GetLastSymbolEndPosition(
		NVGcontextPtr nvg,
		const std::string_view& displayedText,
		vec2 textBoxPosition, vec2 textBoxSize) const;

	void SetColor(vec4 color);
	void SetFontSize(float size);
	void SetFontFamily(FontFamily fontFamily);
	void SetHorizontalAlignment(HorizontalAlignment alignment);
	void SetVerticalAlignment(VerticalAlignment alignment);
	void SetTopPadding(float topMargin);
	void SetBottomPadding(float bottomMargin);
	void SetLeftPadding(float leftMargin);
	void SetRightPadding(float rightMargin);
	void SetTextOffset(float textOffset);

	vec4 GetColor() const;
	float GetFontSize() const;
	FontFamily GetFontFamily() const;
	HorizontalAlignment GetHorizontalAlignment() const;
	VerticalAlignment GetVerticalAlignment() const;
	float GetTopPadding() const;
	float GetBottomPadding() const;
	float GetLeftPadding() const;
	float GetRightPadding() const;
	float GetTextOffset() const;

public:
	void SetTextNvgParameters(NVGcontextPtr nvg) const;
	float NvgGlyphPositionToLocal(float nvgGlyphPosition, vec2 textBoxPosition) const;
	std::vector<NVGglyphPosition> GetNvgGlyphsPosition(
		NVGcontextPtr nvg,
		const std::string_view& displayedText,
		vec2 textBoxPosition) const;

	float m_topPadding;
	float m_bottomPadding;
	float m_leftPadding;
	float m_rightPadding;
	float m_textOffset;

	vec4 m_color;
	float m_fontSize;
	FontFamily m_fontFamily;

	HorizontalAlignment m_horizontalAlignment;
	VerticalAlignment m_verticalAlignment;
};

} // namespace RED_LILIUM_NAMESPACE
