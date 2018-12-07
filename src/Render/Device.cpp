#include "pch.h"
#include "Device.h"
#include "System.h"
#include "DeviceInfo.h"
#include "DeviceMemoryHeap.h"
#include "DeviceMemoryType.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

Device::Device(ptr<System> system, VkPhysicalDevice vkPhysicalDevice)
	: m_system(system)
	, m_vkPhysicalDevice(vkPhysicalDevice)
{
	RED_LILIUM_ASSERT(system != nullptr);

	m_deviceInfo = umake<DeviceInfo>(this);
	InitMemoryHeaps();
}

ptr<DeviceInfo> Device::Info() const
{
	return m_deviceInfo.get();
}

void Device::InitMemoryHeaps()
{
	VkPhysicalDeviceMemoryProperties vkMemoryProperties;
	MemsetZero(&vkMemoryProperties);
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &vkMemoryProperties);

	m_deviceMemoryHeaps.clear();
	for (u32 i = 0; i < vkMemoryProperties.memoryHeapCount; i++)
	{
		auto& heap = vkMemoryProperties.memoryHeaps[i];
		m_deviceMemoryHeaps.push_back(umake<DeviceMemoryHeap>(this, heap, i));
	}

	m_deviceMemoryTypes.clear();
	for (u32 i = 0; i < vkMemoryProperties.memoryTypeCount; i++)
	{
		auto& memoryType = vkMemoryProperties.memoryTypes[i];
		ptr<DeviceMemoryHeap> heap = m_deviceMemoryHeaps[memoryType.heapIndex].get();
		m_deviceMemoryTypes.push_back(umake<DeviceMemoryType>(this, heap, memoryType, i));
	}
}

VkPhysicalDevice Device::GetVkPhysicalDevice() const
{
	return m_vkPhysicalDevice;
}

Device::~Device()
{
	m_deviceMemoryTypes.clear();
	m_deviceMemoryHeaps.clear();
}
