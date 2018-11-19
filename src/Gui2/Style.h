#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class Window : public RedLiliumObject
{
public:
	ptr<Widget> GetTopWidget();
	ptr<const Widget> GetTopWidget() const;

private:
	uptr<Widget> m_topWidget;
};

} // namespace RED_LILIUM_NAMESPACE
