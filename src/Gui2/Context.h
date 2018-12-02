#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class PropertyBase
{};

template<class T>
class Property : public PropertyBase
{

};

class Context : public RedLiliumObject
{
public:

	void SetDirty(ptr<Entity> widget);

	
private:
	std::unordered_set<ptr<Entity>> m_dirtyWidgets;
};

} // namespace RED_LILIUM_NAMESPACE
