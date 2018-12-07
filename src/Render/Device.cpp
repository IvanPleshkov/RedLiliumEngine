#include "pch.h"
#include "Device.h"
#include "System.h"
#include "DeviceInfo.h"
#include "DeviceMemoryHeap.h"
#include "DeviceMemoryType.h"
#include "DeviceQueueFamily.h"
#include "DeviceQueue.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

Device::Device(ptr<System> system, VkPhysicalDevice vkPhysicalDevice)
	: m_system(system)
	, m_vkPhysicalDevice(vkPhysicalDevice)
{
	RED_LILIUM_GUARD;
	RED_LILIUM_ASSERT(system != nullptr);

	m_deviceInfo = umake<DeviceInfo>(this);
	FindLayers();
	FindExtensions();
	InitMemoryHeaps();
	InitQueues();
	SelectLayers();
	SelectExtensions();
	SelectFeatures();
	InitDevice();
}

VkAllocationCallbacks* Device::GetAllocator()
{
	return m_system->GetAllocator();
}

ptr<DeviceInfo> Device::Info() const
{
	return m_deviceInfo.get();
}

void Device::FindLayers()
{
	RED_LILIUM_GUARD;

	m_vkEnabledLayerProperties.clear();
	u32 vkEnabledLayerPropertiesCount;
	vkEnumerateDeviceLayerProperties(m_vkPhysicalDevice, &vkEnabledLayerPropertiesCount, nullptr);
	if (vkEnabledLayerPropertiesCount > 0)
	{
		m_vkEnabledLayerProperties.resize(vkEnabledLayerPropertiesCount);
		vkEnumerateDeviceLayerProperties(m_vkPhysicalDevice, &vkEnabledLayerPropertiesCount, &m_vkEnabledLayerProperties[0]);
	}
}

void Device::SelectLayers()
{
	RED_LILIUM_GUARD;

	m_vkSelectedLayerProperties.clear();
}

void Device::SelectFeatures()
{
	RED_LILIUM_GUARD;

	// VKTODO: select only needed features
	vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_enabledFeatures);
}

void Device::FindExtensions()
{
	RED_LILIUM_GUARD;

	// VKTODO: extensions of layers
	m_vkEnabledExtensionProperties.clear();
	u32 vkEnabledExtensionPropertiesCount;
	vkEnumerateDeviceExtensionProperties(m_vkPhysicalDevice, nullptr, &vkEnabledExtensionPropertiesCount, nullptr);
	if (vkEnabledExtensionPropertiesCount > 0)
	{
		m_vkEnabledExtensionProperties.resize(vkEnabledExtensionPropertiesCount);
		vkEnumerateDeviceExtensionProperties(m_vkPhysicalDevice, nullptr, &vkEnabledExtensionPropertiesCount, &m_vkEnabledExtensionProperties[0]);
	}
}

void Device::SelectExtensions()
{
	RED_LILIUM_GUARD;

	m_vkSelectedExtensionProperties.clear();
}

void Device::InitMemoryHeaps()
{
	RED_LILIUM_GUARD;

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

void Device::InitQueues()
{
	RED_LILIUM_GUARD;

	u32 vkQueueFamilyPropertiesCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &vkQueueFamilyPropertiesCount, nullptr);
	RED_LILIUM_ASSERT(vkQueueFamilyPropertiesCount > 0);

	std::vector<VkQueueFamilyProperties> vkQueueFamilyProperties(vkQueueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &vkQueueFamilyPropertiesCount, &vkQueueFamilyProperties[0]);

	m_deviceQueueFamilies.clear();
	for (u32 i = 0; i < vkQueueFamilyPropertiesCount; i++)
	{
		m_deviceQueueFamilies.push_back(umake<DeviceQueueFamily>(this, vkQueueFamilyProperties[i], i));
	}
}

void Device::InitDevice()
{
	RED_LILIUM_GUARD;

	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
	for (const auto& queueFamily : m_deviceQueueFamilies)
	{
		VkDeviceQueueCreateInfo deviceQueueCreateInfo;
		MemsetZero(&deviceQueueCreateInfo);
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.pNext = nullptr;
		deviceQueueCreateInfo.flags = 0;
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily->GetVkIndex();
		deviceQueueCreateInfo.queueCount = queueFamily->GetQueuesCount();
		deviceQueueCreateInfo.pQueuePriorities = nullptr; // VKTODO: investigate queue priorities, maybe helpfull
		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
	}

	std::vector<const char*> ppLayerNames;
	for (auto& layer : m_vkSelectedLayerProperties)
	{
		ppLayerNames.push_back(layer.layerName);
	}

	std::vector<const char*> ppExtensionNames;
	for (auto& extension : m_vkSelectedExtensionProperties)
	{
		ppExtensionNames.push_back(extension.extensionName);
	}

	VkDeviceCreateInfo vkDeviceCreateInfo;
	MemsetZero(&vkDeviceCreateInfo);
	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vkDeviceCreateInfo.pNext = nullptr;
	vkDeviceCreateInfo.flags = 0;
	vkDeviceCreateInfo.queueCreateInfoCount = static_cast<u32>(deviceQueueCreateInfos.size());
	vkDeviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfos[0];
	vkDeviceCreateInfo.enabledLayerCount = static_cast<u32>(ppLayerNames.size());
	vkDeviceCreateInfo.ppEnabledLayerNames = ppLayerNames.size() ? &ppLayerNames[0] : nullptr;
	vkDeviceCreateInfo.enabledExtensionCount = static_cast<u32>(ppExtensionNames.size());
	vkDeviceCreateInfo.ppEnabledExtensionNames = ppExtensionNames.size() ? &ppExtensionNames[0] : nullptr;
	vkDeviceCreateInfo.pEnabledFeatures = &m_enabledFeatures;

	VkResult result = vkCreateDevice(
		m_vkPhysicalDevice,
		&vkDeviceCreateInfo,
		GetAllocator(),
		&m_vkDevice);

	RED_LILIUM_ASSERT(result == VK_SUCCESS);
}

VkPhysicalDevice Device::GetVkPhysicalDevice() const
{
	return m_vkPhysicalDevice;
}

VkDevice Device::GetVkDevice() const
{
	return m_vkDevice;
}

Device::~Device()
{
	RED_LILIUM_GUARD;

	if (m_vkDevice != VK_NULL_HANDLE)
	{
		vkDeviceWaitIdle(m_vkDevice);
		vkDestroyDevice(m_vkDevice, GetAllocator());
		m_deviceQueueFamilies.clear();
		m_deviceMemoryTypes.clear();
		m_deviceMemoryHeaps.clear();
		m_deviceInfo.reset();
		m_vkEnabledLayerProperties.clear();
		m_vkEnabledExtensionProperties.clear();
		m_vkSelectedLayerProperties.clear();
		m_vkSelectedExtensionProperties.clear();
		m_vkDevice = VK_NULL_HANDLE;
		m_vkPhysicalDevice = VK_NULL_HANDLE;
		m_system = nullptr;
	}
}
