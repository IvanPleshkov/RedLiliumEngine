#pragma once

#include "core/common.h"
#include "Layout.h"

namespace RED_LILIUM_NAMESPACE
{

class CanvasLayout : public LayoutWidget
{
public:
	CanvasLayout() {}
	~CanvasLayout() override {}

private:
	mat3 m_transform;
	std::unordered_map<ptr<Widget>, Rect> m_childrensRect;
};

}  // namespace RED_LILIUM_NAMESPACE
