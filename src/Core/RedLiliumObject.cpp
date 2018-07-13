#include "pch.h"

#include "RedLiliumObject.h"
#include "SmartPointers.h"

using namespace RED_LILIUM_NAMESPACE;

#if RED_LILIUM_POINTER_COUNTERS
sptr<RedLiliumPtrHandler> RedLiliumObject::m_nullHandler = std::make_shared<RedLiliumPtrHandler>(nullptr);
#endif // RED_LILIUM_POINTER_COUNTERS

RedLiliumObject::~RedLiliumObject()
{
#if RED_LILIUM_POINTER_COUNTERS
	m_handler->exists = false;
#endif // RED_LILIUM_POINTER_COUNTERS
}
