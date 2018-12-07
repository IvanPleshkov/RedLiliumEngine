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
	VkAllocationCallbacks* GetAllocator();
	VkPhysicalDevice GetVkPhysicalDevice() const;
	VkDevice GetVkDevice() const;

private:
	void FindExtensions();
	void SelectExtensions();
	void FindLayers();
	void SelectLayers();
	void SelectFeatures();
	void InitMemoryHeaps();
	void InitQueues();
	void InitDevice();

	ptr<System>								m_system;
	VkDevice								m_vkDevice;
	VkPhysicalDevice						m_vkPhysicalDevice;
	VkPhysicalDeviceFeatures				m_enabledFeatures;
	uptr<DeviceInfo>						m_deviceInfo;
	std::vector<uptr<DeviceMemoryHeap>>		m_deviceMemoryHeaps;
	std::vector<uptr<DeviceMemoryType>>		m_deviceMemoryTypes;
	std::vector<uptr<DeviceQueueFamily>>	m_deviceQueueFamilies;
	std::vector<VkLayerProperties>			m_vkEnabledLayerProperties;
	std::vector<VkExtensionProperties>		m_vkEnabledExtensionProperties;
	std::vector<VkLayerProperties>			m_vkSelectedLayerProperties;
	std::vector<VkExtensionProperties>		m_vkSelectedExtensionProperties;
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
