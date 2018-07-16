#include "pch.h"

#include "RedLiliumObject.h"
#include "SmartPointers.h"

using namespace RED_LILIUM_NAMESPACE;

#if RED_LILIUM_POINTER_COUNTERS
sptr<std::atomic_bool> RedLiliumObject::m_nullExistingHandler = std::make_shared<std::atomic_bool>(true);
#endif // RED_LILIUM_POINTER_COUNTERS

RedLiliumObject::~RedLiliumObject()
{
#if RED_LILIUM_POINTER_COUNTERS
	*m_existingHandler = false;
#endif // RED_LILIUM_POINTER_COUNTERS
}
