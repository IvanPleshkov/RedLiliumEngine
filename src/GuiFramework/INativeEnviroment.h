#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

/*interface*/ class INativeEnvironment
{
public:
	virtual ~INativeEnvironment() {}

	virtual Time GetDoubleClickingTime() const = 0;
};

} // namespace RED_LILIUM_NAMESPACE
