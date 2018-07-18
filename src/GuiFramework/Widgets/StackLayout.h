#pragma once

#include "core/common.h"
#include "Layout.h"

namespace RED_LILIUM_NAMESPACE
{

class StackLayout : public LayoutWidget
{
public:
	ptr<StackLayout> SetPadding(vec2 padding);
	ptr<StackLayout> SetHorizontalAlignment(HorizontalAlignment alignment);
	ptr<StackLayout> SetVerticalAlignment(VerticalAlignment alignment);
	ptr<StackLayout> SetNormalization(bool useXNormalization, bool useYNormalization);
	ptr<StackLayout> SetDirection(Direction direction);

protected:
	void UpdateDesiredSize() override;
	void UpdateMinimumSize() override;
	void Layout() override;

private:
	using CoordinateTaker = std::function<float& (vec2&)>;
	static const CoordinateTaker& XCoordinateTaker();
	static const CoordinateTaker& YCoordinateTaker();

	void UpdateDesiredSize(const CoordinateTaker& fx, const CoordinateTaker& fy);
	void UpdateMinimumSize(const CoordinateTaker& fx, const CoordinateTaker& fy);

	void LayoutX(
		const CoordinateTaker& fx,
		HorizontalAlignment alignment,
		bool isReverted,
		bool normalise);
	void LayoutY(
		const CoordinateTaker& fy,
		VerticalAlignment alignment,
		bool isReverted,
		bool normalise);

	bool m_useXNormalization = false;
	bool m_useYNormalization = false;
	vec2 m_padding = { 0, 0 };
	HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Fill;
	VerticalAlignment m_verticalAlignment = VerticalAlignment::Top;
	Direction m_direction = Direction::Vertical;
};

}  // namespace RED_LILIUM_NAMESPACE
