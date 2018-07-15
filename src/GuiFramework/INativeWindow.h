#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

/*interface*/ class INativeWindow
{
public:

	virtual ~INativeWindow() {}

	virtual vec2 GetSize() const = 0;
	virtual bool IsFocused() const = 0;
	virtual NVGcontextPtr GetNvgContext() = 0;
	virtual const NVGcontextPtr GetNvgContext() const = 0;
};

} // namespace RED_LILIUM_NAMESPACE
