#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class DeviceInfo : public RedLiliumObject
{
public:
	DeviceInfo(ptr<Device> device);
	~DeviceInfo() override;

private:
	ptr<Device> m_device;
	VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
	// VKTODO add VkFormatProperties for all formats
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
