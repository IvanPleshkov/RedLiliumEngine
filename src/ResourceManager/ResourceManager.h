#pragma once

#include "ResourceManagerForward.h"
#include <TaskManager/AsyncReturn.h>

namespace RED_LILIUM_NAMESPACE
{

class ResourceManager : public RedLiliumObject
{
public:
	~ResourceManager() override { }

	template<class TResource, class TResourceInitParameters>
	AsyncResult<ptr<const TResource>> CreateAsync(const TResourceInitParameters& initParameters) const;

	template<class TResource>
	ptr<TResource> Create();

	void CollectGarbage();

protected:
	std::unordered_set<sptr<Resource>> m_resources;
};

} // namespace RED_LILIUM_NAMESPACE
