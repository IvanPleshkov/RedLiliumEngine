#pragma once

#include "ResourceManagerForward.h"
#include <TaskManager/AsyncReturn.h>

namespace RED_LILIUM_NAMESPACE
{

class ResourceManager : public RedLiliumObject
{
public:
	~ResourceManager() override { }

	void CollectGarbage();

protected:
	std::unordered_set<sptr<IResource>> m_resources;
};

} // namespace RED_LILIUM_NAMESPACE
