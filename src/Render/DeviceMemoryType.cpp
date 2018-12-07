#include "pch.h"
#include "DeviceMemoryType.h"
#include "DeviceMemoryHeap.h"
#include "System.h"
#include "Device.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

DeviceMemoryType::DeviceMemoryType(
	ptr<Device> device,
	ptr<DeviceMemoryHeap> heap,
	const VkMemoryType& vkMemoryType,
	u32 vkTypeIndex)
	: m_device(device)
	, m_heap(heap)
	, m_vkMemoryType(vkMemoryType)
	, m_vkTypeIndex(vkTypeIndex)
{}

DeviceMemoryType::~DeviceMemoryType()
{}

bool DeviceMemoryType::IsDeviceLocal() const
{
	return m_vkMemoryType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

bool DeviceMemoryType::IsHostVisible() const
{
	return m_vkMemoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
}

bool DeviceMemoryType::IsHostCoherent() const
{
	return m_vkMemoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}

bool DeviceMemoryType::IsHostCached() const
{
	return m_vkMemoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
}

bool DeviceMemoryType::IsLazilyAllocated() const
{
	return m_vkMemoryType.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
}
