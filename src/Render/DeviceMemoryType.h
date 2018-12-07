#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class DeviceMemoryType : public RedLiliumObject
{
public:
	~DeviceMemoryType() override;

	bool IsDeviceLocal() const;
	bool IsHostVisible() const;
	bool IsHostCoherent() const;
	bool IsHostCached() const;
	bool IsLazilyAllocated() const;

RED_LILIUM_INTERNAL:
	DeviceMemoryType(
		ptr<Device> device,
		ptr<DeviceMemoryHeap> heap,
		const VkMemoryType& vkMemoryType,
		u32 vkTypeIndex);

private:
	ptr<Device> m_device;
	ptr<DeviceMemoryHeap> m_heap;
	VkMemoryType m_vkMemoryType;
	u32 m_vkTypeIndex;
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
