#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

/*interface*/ class INativeWindow
{
	virtual bool IsFocused() const = 0;
	virtual NVGcontextPtr GetNvgContext() = 0;
	virtual const NVGcontextPtr GetNvgContext() const = 0;
};

} // namespace RED_LILIUM_NAMESPACE

#include "Widget.hpp"
