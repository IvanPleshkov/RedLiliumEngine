#pragma once

#include "StdInclude.h"

namespace RED_LILIUM_NAMESPACE
{

class RedLiliumObject
{
public:
	//RedLiliumObject(const RedLiliumObject&) = delete;
	//RedLiliumObject& operator=(const RedLiliumObject& r) = delete;
	virtual ~RedLiliumObject();

#if RED_LILIUM_POINTER_COUNTERS
	static sptr<std::atomic_bool> m_nullExistingHandler;
	sptr<std::atomic_bool> m_existingHandler = std::make_shared<std::atomic_bool>(true);
#endif // RED_LILIUM_POINTER_COUNTERS
};

} // RED_LILIUM_NAMESPACE
