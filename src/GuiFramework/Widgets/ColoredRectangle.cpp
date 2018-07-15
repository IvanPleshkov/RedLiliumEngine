#include "pch.h"
#include "ColoredRectangle.h"

using namespace RED_LILIUM_NAMESPACE;

ColoredRectangle::ColoredRectangle()
{
	SetHovering(true);
}

ptr<ColoredRectangle> ColoredRectangle::SetColor(vec4 color)
{
	m_color = color;
	return this;
}

ptr<ColoredRectangle> ColoredRectangle::SetMinimumSize(vec2 minimumSize)
{
	m_rectMinimumSize = minimumSize;
	return this;
}

ptr<ColoredRectangle> ColoredRectangle::SetDesiredSize(vec2 desiredSize)
{
	m_rectDesiredSize = desiredSize;
	return this;
}

void ColoredRectangle::UpdateDesiredSize()
{
	m_desiredSize = m_rectDesiredSize;
}

void ColoredRectangle::UpdateMinimumSize()
{
	m_minimumSize = m_rectMinimumSize;
}

void ColoredRectangle::Draw()
{
	vec2 position = GetPosition();
	vec2 size = GetSize();
	NVGcolor color = nvgRGBAf(m_color.x, m_color.y, m_color.z, m_color.a);

	if (IsHovered())
	{
		color = nvgRGBAf(1, 0, 0, 1);
	}

	nvgBeginPath(GetNvgContext());
	nvgRect(GetNvgContext(), position.x, position.y, size.x, size.y);
	nvgFillColor(GetNvgContext(), color);
	nvgFill(GetNvgContext());
}
