#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class DeviceMemoryHeap : public RedLiliumObject
{
public:
	~DeviceMemoryHeap() override;

	bool IsDeviceLocal() const;

RED_LILIUM_INTERNAL:
	DeviceMemoryHeap(
		ptr<Device> device,
		const VkMemoryHeap& vkMemoryHeap,
		u32 vkHeapIndex);

private:
	ptr<Device> m_device;
	VkMemoryHeap m_vkMemoryHeap;
	u32 m_vkHeapIndex;
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
