#include "pch.h"
#include "TextField.h"

using namespace RED_LILIUM_NAMESPACE;

TextField::TextField()
	: m_textWidgetsHelper(std::move(std::make_unique<TextWidgetsHelper>()))
	, m_disaplyedText()
	, m_cursorPosition(m_disaplyedText.end())
	, m_cursorDrawPosition(0)
{}

ptr<TextField> TextField::SetText(const std::string& text)
{
	if (m_text != text)
	{
		m_text = text;
		ResetText();
	}
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
	m_textWidgetsHelper->Draw(GetNvgContext(), m_disaplyedText, GetPosition(), GetSize());

	if (IsFocused())
	{
		DrawCursor();
	}
}

void TextField::DrawCursor()
{
	nvgBeginPath(GetNvgContext());
	nvgRect(GetNvgContext(), m_cursorDrawPosition - 1, 0, 2, GetSize().y);
	nvgFillColor(GetNvgContext(), nvgRGBA(0xff, 0x00, 0x00, 0xff));
	nvgFill(GetNvgContext());
}

void TextField::UpdateDesiredSize()
{
	m_desiredSize = m_textWidgetsHelper->GetDisplayedTextSize(GetNvgContext(), m_disaplyedText);
}

void TextField::ResetText()
{
	m_disaplyedText = m_text;
	SetCursorPosition(m_disaplyedText.end(), false);
}

void TextField::SetCursorPosition(std::string::iterator cursorPosition, bool saveSelection)
{
	RED_LILIUM_ASSERT(cursorPosition >= m_disaplyedText.begin() && cursorPosition <= m_disaplyedText.end());
	m_cursorPosition = cursorPosition;

	// update m_cursorDrawPosition
	if (m_cursorPosition != m_disaplyedText.end())
	{
		const u32 symbolIndex = static_cast<u32>(std::distance(m_disaplyedText.begin(), m_cursorPosition));
		m_cursorDrawPosition = m_textWidgetsHelper->GetSymbolGlyphBeginPosition(
			GetNvgContext(), m_disaplyedText, GetPosition(), GetSize(), symbolIndex);
	}
	else
	{
		m_cursorDrawPosition = m_textWidgetsHelper->GetLastSymbolEndPosition(
			GetNvgContext(), m_disaplyedText, GetPosition(), GetSize());
	}
}

void TextField::InsertText(const std::string_view& text)
{
	RED_LILIUM_ASSERT(m_cursorPosition >= m_disaplyedText.begin() && m_cursorPosition <= m_disaplyedText.end());
}

bool TextField::HandleKeyEvent(const KeyEvent& keyEvent)
{
	return false;
}

void TextField::OnClick(MouseKey mouseKey)
{
	auto mousePositionOpt = GetLocalMousePosition();
	if (!mousePositionOpt)
	{
		SetCursorPosition(m_disaplyedText.end(), false);
		return;
	}

	std::optional<u32> hoveredSymbolOpt = m_textWidgetsHelper->GetSymbolUnderPoint(
		GetNvgContext(), m_disaplyedText, { 0, 0 }, m_size, mousePositionOpt.value());

	if (hoveredSymbolOpt)
	{
		SetCursorPosition(m_disaplyedText.begin() + hoveredSymbolOpt.value(), false);
	}
	else
	{
		SetCursorPosition(m_disaplyedText.end(), false);
	}
}
