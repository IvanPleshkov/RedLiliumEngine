#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class Allocator
{
};

class System : public RedLiliumObject, public std::enable_shared_from_this<System>
{
public:
	System();
	~System() override;

/*internal*/ public:
	VkAllocationCallbacks* GetAllocator();

private:
	void InitDevices();

	VkInstance m_vkInstance;
	std::vector<uptr<Device>> m_devices;
};

}
} // namespace RED_LILIUM_NAMESPACE
