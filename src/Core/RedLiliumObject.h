#pragma once

#include "StdInclude.h"

namespace RED_LILIUM_NAMESPACE
{

#if RED_LILIUM_POINTER_COUNTERS
class RedLiliumObject;
struct RedLiliumPtrHandler
{
	RedLiliumPtrHandler(RedLiliumObject* pointer)
		: ptr(pointer)
		, exists(true)
	{}
	RedLiliumObject* ptr;
	std::atomic_bool exists;
};
#endif // RED_LILIUM_POINTER_COUNTERS

class RedLiliumObject
{
public:
	//RedLiliumObject(const RedLiliumObject&) = delete;
	//RedLiliumObject& operator=(const RedLiliumObject& r) = delete;
	virtual ~RedLiliumObject();

#if RED_LILIUM_POINTER_COUNTERS
	static sptr<RedLiliumPtrHandler> m_nullHandler;
	sptr<RedLiliumPtrHandler> m_handler = std::make_shared<RedLiliumPtrHandler>(this);
#endif // RED_LILIUM_POINTER_COUNTERS
};

} // RED_LILIUM_NAMESPACE
