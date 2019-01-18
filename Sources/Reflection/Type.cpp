#include "pch.h"
#include "Type.h"

using namespace RED_LILIUM_NAMESPACE;

uptr<Serializable> Type::Construct() const
{
	return m_constructor();
}
