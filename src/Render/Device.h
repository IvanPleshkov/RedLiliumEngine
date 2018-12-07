#pragma once

#include "RenderForward.h"
#include "DeviceInfo.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class Device : public RedLiliumObject
{
public:
	~Device() override;
	ptr<DeviceInfo> Info() const;

RED_LILIUM_INTERNAL:
	Device(ptr<System> system, VkPhysicalDevice vkPhysicalDevice);
	VkPhysicalDevice GetVkPhysicalDevice() const;

private:
	void InitMemoryHeaps();

	uptr<DeviceInfo> m_deviceInfo;
	ptr<System> m_system;
	VkPhysicalDevice m_vkPhysicalDevice;
	std::vector<uptr<DeviceMemoryHeap>> m_deviceMemoryHeaps;
	std::vector<uptr<DeviceMemoryType>> m_deviceMemoryTypes;
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
