#include "pch.h"
#include "System.h"
#include "Device.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

System::System()
{
	FindLayers();
	FindExtensions();

	SelectLayers();
	SelectExtensions();

	// VKTODO: application info
	VkApplicationInfo appInfo;
	MemsetZero(&appInfo);
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Demo";
	appInfo.applicationVersion = 1;
	appInfo.apiVersion = VK_API_VERSION_1_1;

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

	VkInstanceCreateInfo instanceCreateInfo;
	MemsetZero(&instanceCreateInfo);
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(ppExtensionNames.size());
	instanceCreateInfo.ppEnabledExtensionNames = ppExtensionNames.size() ? &ppExtensionNames[0] : nullptr;
	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(ppLayerNames.size());
	instanceCreateInfo.ppEnabledLayerNames = ppLayerNames.size() ? &ppLayerNames[0] : nullptr;

	VkResult result = vkCreateInstance(&instanceCreateInfo, GetAllocator(), &m_vkInstance);
	RED_LILIUM_ASSERT(result == VK_SUCCESS);

	InitDevices();
}

System::~System()
{
	if (m_vkInstance != VK_NULL_HANDLE)
	{
		m_devices.clear();
		vkDestroyInstance(m_vkInstance, nullptr);
		m_vkEnabledLayerProperties.clear();
		m_vkEnabledExtensionProperties.clear();
		m_vkSelectedLayerProperties.clear();
		m_vkSelectedExtensionProperties.clear();
		m_vkInstance = VK_NULL_HANDLE;
	}
}

void System::InitDevices()
{
	u32 physicalDevicesCount = 0;
	VkResult result = vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, nullptr);
	RED_LILIUM_ASSERT(result == VK_SUCCESS);

	std::vector<VkPhysicalDevice> physicalDevicesHandle(physicalDevicesCount);
	result = vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, &physicalDevicesHandle[0]);
	RED_LILIUM_ASSERT(result == VK_SUCCESS);

	m_devices.clear();
	for (auto& deviceHandle : physicalDevicesHandle)
	{
		m_devices.push_back(umake<Device>(this, deviceHandle));
	}
}

void System::FindExtensions()
{
	// VKTODO: extensions of layers
	u32 extensionPropertiesCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, nullptr);
	if (extensionPropertiesCount > 0)
	{
		m_vkEnabledExtensionProperties.resize(extensionPropertiesCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, &m_vkEnabledExtensionProperties[0]);
	}
}

void System::SelectExtensions()
{
	m_vkSelectedExtensionProperties.clear();
}

void System::FindLayers()
{
	u32 vkInstancePropertiesCount;
	vkEnumerateInstanceLayerProperties(&vkInstancePropertiesCount, nullptr);
	if (vkInstancePropertiesCount > 0)
	{
		m_vkEnabledLayerProperties.resize(vkInstancePropertiesCount);
		vkEnumerateInstanceLayerProperties(&vkInstancePropertiesCount, &m_vkEnabledLayerProperties[0]);
	}
}

void System::SelectLayers()
{
	m_vkSelectedLayerProperties.clear();
}

VkInstance System::GetVkInstance() const
{
	return m_vkInstance;
}

VkAllocationCallbacks* System::GetAllocator()
{
	return nullptr;
}
