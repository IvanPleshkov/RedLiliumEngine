#include "pch.h"
#include "DeviceMemoryHeap.h"
#include "System.h"
#include "Device.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

DeviceMemoryHeap::DeviceMemoryHeap(
	ptr<Device> device,
	const VkMemoryHeap& vkMemoryHeap,
	u32 vkHeapIndex)
	: m_device(device)
	, m_vkMemoryHeap(vkMemoryHeap)
	, m_vkHeapIndex(vkHeapIndex)
{}

DeviceMemoryHeap::~DeviceMemoryHeap()
{}

bool DeviceMemoryHeap::IsDeviceLocal() const
{
	return m_vkMemoryHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;
}
