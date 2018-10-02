#pragma once

#include "ResourceManagerForward.h"

namespace RED_LILIUM_NAMESPACE
{

class Resource : public RedLiliumObject
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
	~Resource() override {}

	virtual const std::string& Name() const = 0;

protected:
	std::atomic<Status> m_status;
};

} // namespace RED_LILIUM_NAMESPACE
