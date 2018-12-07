#include "pch.h"
#include "DeviceInfo.h"
#include "System.h"
#include "Device.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

DeviceInfo::DeviceInfo(ptr<Device> device)
	: m_device(device)
{
	VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device->GetVkPhysicalDevice(), &vkPhysicalDeviceProperties);
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(device->GetVkPhysicalDevice(), &vkPhysicalDeviceFeatures);
}

DeviceInfo::~DeviceInfo()
{
}
