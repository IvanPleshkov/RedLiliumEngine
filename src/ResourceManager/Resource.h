#pragma once

#include "ResourceManagerForward.h"

namespace RED_LILIUM_NAMESPACE
{

class IResource {};

template<class TResourceType, class TResourceManager>
class Resource : public RedLiliumObject, public IResource
{
public:

	enum class Status : u8
	{
		Pending		= 1 << 0,
		Loading		= 1 << 1,
		Loaded		= 1 << 2,
		Failed		= 1 << 3,
		Unknown		= 0
	};

public:
	Resource() {}
	~Resource() override {}

	const std::string& Name() const;

	template<class TModifyParameters>
	void Modify(const TModifyParameters& modifyParameters) const;

	template<class TModifyParameters>
	void Modify(std::function<void(ptr<TResourceType>)>) const;

protected:
	std::string m_name;
	Status m_status;
	ptr<TResourceManager> m_resourceManager;
};

} // namespace RED_LILIUM_NAMESPACE
