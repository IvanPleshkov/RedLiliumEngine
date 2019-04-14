#pragma once

#include <Core/Common.h>
#include <ComponentContainer.h>

namespace RED_LILIUM_NAMESPACE
{

class MetaClass final
{
public:
	MetaClass();
	MetaClass(MetaClass parent, ComponentTypeId componentType);

private:
	std::vector<ptr<MetaClass>> m_children;
};

} // namespace RED_LILIUM_NAMESPACE
