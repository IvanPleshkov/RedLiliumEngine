#include "pch.h"
#include "DeviceQueueFamily.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

DeviceQueueFamily::DeviceQueueFamily(ptr<Device> device, VkQueueFamilyProperties vkQueueFamilyProperties, u32 vkIndex)
	: m_device(device)
	, m_vkIndex(vkIndex)
	, m_vkQueueFamilyProperties(vkQueueFamilyProperties)
{}

DeviceQueueFamily::~DeviceQueueFamily()
{}

u32 DeviceQueueFamily::GetQueuesCount() const
{
	return m_vkQueueFamilyProperties.queueCount;
}

bool DeviceQueueFamily::SupportsRendering() const
{
	return m_vkQueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

bool DeviceQueueFamily::SupportsComputeShaders() const
{
	return m_vkQueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT;
}

bool DeviceQueueFamily::SupportsTransfer() const
{
	return m_vkQueueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT;
}

bool DeviceQueueFamily::SupportsSparseBinding() const
{
	return m_vkQueueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT;
}

bool DeviceQueueFamily::SupportsTimestamps() const
{
	return m_vkQueueFamilyProperties.timestampValidBits > 0;
}

u32 DeviceQueueFamily::GetVkIndex() const
{
	return m_vkIndex;
}

const VkQueueFamilyProperties& DeviceQueueFamily::GetVkQueueFamilyProperties() const
{
	return m_vkQueueFamilyProperties;
}
