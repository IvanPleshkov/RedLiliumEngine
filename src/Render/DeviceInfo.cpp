#include "pch.h"
#include "DeviceInfo.h"
#include "System.h"
#include "Device.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

DeviceInfo::DeviceInfo(ptr<Device> device)
	: m_device(device)
{
	vkGetPhysicalDeviceProperties(device->GetVkPhysicalDevice(), &m_vkPhysicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(device->GetVkPhysicalDevice(), &m_vkPhysicalDeviceFeatures);
}

DeviceInfo::~DeviceInfo()
{
}
