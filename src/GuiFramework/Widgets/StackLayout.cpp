#include "pch.h"
#include "StackLayout.h"

using namespace RED_LILIUM_NAMESPACE;

ptr<StackLayout> StackLayout::SetPadding(vec2 padding)
{
	m_padding = padding;
	return this;
}

ptr<StackLayout> StackLayout::SetHorizontalAlignment(HorizontalAlignment alignment)
{
	m_horizontalAlignment = alignment;
	return this;
}

ptr<StackLayout> StackLayout::SetVerticalAlignment(VerticalAlignment alignment)
{
	m_verticalAlignment = alignment;
	return this;
}

ptr<StackLayout> StackLayout::SetNormalization(bool useXNormalization, bool useYNormalization)
{
	m_useXNormalization = useXNormalization;
	m_useYNormalization = useYNormalization;
	return this;
}

ptr<StackLayout> StackLayout::SetDirection(Direction direction)
{
	m_direction = direction;
	return this;
}

void StackLayout::UpdateDesiredSize()
{
	switch (m_direction)
	{
	case Direction::Horizontal:
	case Direction::RevertedHorizontal:
		UpdateDesiredSize(YCoordinateTaker(), XCoordinateTaker());
		break;
	case Direction::Vertical:
	case Direction::RevertedVertical:
		UpdateDesiredSize(XCoordinateTaker(), YCoordinateTaker());
		break;
	}
}

void StackLayout::UpdateMinimumSize()
{
	switch (m_direction)
	{
	case Direction::Vertical:
	case Direction::RevertedVertical:
		UpdateMinimumSize(XCoordinateTaker(), YCoordinateTaker());
		break;
	case Direction::Horizontal:
	case Direction::RevertedHorizontal:
		UpdateMinimumSize(YCoordinateTaker(), XCoordinateTaker());
		break;
	}
}

void StackLayout::Layout()
{
	HorizontalAlignment convertedHorAlignment = static_cast<HorizontalAlignment>(m_verticalAlignment);
	VerticalAlignment convertedVerAlignment = static_cast<VerticalAlignment>(m_horizontalAlignment);
	switch (m_direction)
	{
	case Direction::Vertical:
		LayoutX(XCoordinateTaker(), m_horizontalAlignment, false, m_useXNormalization);
		LayoutY(YCoordinateTaker(), m_verticalAlignment, false, m_useYNormalization);
		break;
	case Direction::RevertedVertical:
		LayoutX(XCoordinateTaker(), m_horizontalAlignment, true, m_useXNormalization);
		LayoutY(YCoordinateTaker(), m_verticalAlignment, true, m_useYNormalization);
		break;
	case Direction::Horizontal:
		LayoutX(YCoordinateTaker(), convertedHorAlignment, false, m_useYNormalization);
		LayoutY(XCoordinateTaker(), convertedVerAlignment, false, m_useXNormalization);
		break;
	case Direction::RevertedHorizontal:
		LayoutX(YCoordinateTaker(), convertedHorAlignment, true, m_useYNormalization);
		LayoutY(XCoordinateTaker(), convertedVerAlignment, true, m_useXNormalization);
		break;
	}
}

const StackLayout::CoordinateTaker& StackLayout::XCoordinateTaker()
{
	static CoordinateTaker xCoordinate = [](vec2& v) -> float& { return v.x; };
	return xCoordinate;
}

const StackLayout::CoordinateTaker& StackLayout::YCoordinateTaker()
{
	static CoordinateTaker yCoordinate = [](vec2& v) -> float& { return v.y; };
	return yCoordinate;
}

void StackLayout::UpdateDesiredSize(
	const StackLayout::CoordinateTaker& fx,
	const StackLayout::CoordinateTaker& fy)
{
	m_desiredSize = { 0, 0 };

	for (auto& child : m_children)
	{
        vec2 childDesiredSize = child->GetDesiredSize();
        fx(m_desiredSize) = std::max(fx(m_desiredSize), fx(childDesiredSize));
        fy(m_desiredSize) += fy(childDesiredSize);
		fy(m_desiredSize) += fy(m_padding);
	}

	fx(m_desiredSize) += 2.0f * fx(m_padding);
	fy(m_desiredSize) += fy(m_padding);
}

void StackLayout::UpdateMinimumSize(
	const StackLayout::CoordinateTaker& fx,
	const StackLayout::CoordinateTaker& fy)
{
	m_minimumSize = { 0, 0 };

	for (auto& child : m_children)
	{
        vec2 childMinimumSize = child->GetMinimumSize();
        fx(m_minimumSize) = std::max(fx(m_minimumSize), fx(childMinimumSize));
        fy(m_minimumSize) += fy(childMinimumSize);
		fy(m_minimumSize) += fy(m_padding);
	}

	fx(m_minimumSize) += 2.0f * fx(m_padding);
	fy(m_minimumSize) += fy(m_padding);
}

void StackLayout::LayoutX(
	const StackLayout::CoordinateTaker& fx,
	HorizontalAlignment alignment,
	bool isReverted,
	bool normalise)
{
    vec2 thisSize = GetSize();
    const float maxChildSizeX = fx(thisSize) - 2.0f * fx(m_padding);
	for (size_t i = 0; i < m_children.size(); i++)
	{
		size_t j = isReverted ? (m_children.size() - i - 1) : i;
		auto& child = m_children[j];

		// Child Size
		float childSizeX;
		if (alignment == HorizontalAlignment::Fill)
		{
			childSizeX = maxChildSizeX;
		}
		else
		{
			if (normalise)
			{
                vec2 thisDesiredSize = this->GetDesiredSize();
                childSizeX = fx(thisDesiredSize) - 2.0f * fx(m_padding);
			}
			else
			{
                vec2 childDesiredSize = child->GetDesiredSize();
                childSizeX = fx(childDesiredSize);
			}
			childSizeX = std::min(childSizeX, maxChildSizeX);
		}

		vec2 childSize = child->GetSize();
		fx(childSize) = childSizeX;
		child->SetSize(childSize);

		// Child Position
		float childPositionX = fx(m_padding);
		switch (alignment)
		{
		case HorizontalAlignment::Fill:
		case HorizontalAlignment::Left:
			break;
		case HorizontalAlignment::Center:
			childPositionX += maxChildSizeX / 2 - childSizeX / 2;
			break;
		case HorizontalAlignment::Right:
			childPositionX += maxChildSizeX - childSizeX;
			break;
		}

		vec2 childPosition = child->GetPosition();
		fx(childPosition) = childPositionX;
		child->SetPosition(childPosition);
	}
}

void StackLayout::LayoutY(
	const StackLayout::CoordinateTaker& fy,
	VerticalAlignment alignment,
	bool isReverted,
	bool normalise)
{
	// TODO: implement
	RED_LILIUM_ASSERT(!normalise && "Not implemented");
	RED_LILIUM_ASSERT(alignment != VerticalAlignment::Fill && "Not implemented");
	RED_LILIUM_ASSERT(alignment != VerticalAlignment::Center && "Not implemented");

	float childYSizeAccumulator = 0;
	for (size_t i = 0; i < m_children.size(); i++)
	{
		size_t j = isReverted ? (m_children.size() - i - 1) : i;
		auto& child = m_children[j];

		// Child size
        vec2 childDesiredSize = child->GetDesiredSize();
        const float childSizeY = fy(childDesiredSize);
		vec2 childSize = child->GetSize();
		fy(childSize) = childSizeY;
		child->SetSize(childSize);

		// Child Position
		float childPositionY = fy(m_padding);
		switch (alignment)
		{
		case VerticalAlignment::Fill:
			break;
        case VerticalAlignment::Top:
            childPositionY += childYSizeAccumulator;
            break;
        case VerticalAlignment::Bottom:
            {
                vec2 thisSize = this->GetSize();
                childPositionY += fy(thisSize) - fy(m_padding) - childYSizeAccumulator;
            }
            break;
        case VerticalAlignment::Center:
            break;
		}

		vec2 childPosition = child->GetPosition();
		fy(childPosition) = childPositionY;
		child->SetPosition(childPosition);
        childYSizeAccumulator += fy(childSize) + fy(m_padding);
	}
}
