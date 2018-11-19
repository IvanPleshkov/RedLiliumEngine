#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class WidgetComponent : public RedLiliumObject
{};

class Widget : public RedLiliumObject
{
public:

	template<class TComponent>
	ptr<TComponent> GetComponent();

private:
	std::vector<uptr<WidgetComponent>> m_components;
};

} // namespace RED_LILIUM_NAMESPACE
