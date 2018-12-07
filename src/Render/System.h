#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class System : public RedLiliumObject
{
public:
	System();
	~System() override;

RED_LILIUM_INTERNAL:
	VkInstance GetVkInstance() const;
	VkAllocationCallbacks* GetAllocator();

private:
	void InitDevices();
	void FindExtensions();
	void SelectExtensions();
	void FindLayers();
	void SelectLayers();

	VkInstance m_vkInstance;
	std::vector<uptr<Device>> m_devices;
	std::vector<VkLayerProperties>			m_vkEnabledLayerProperties;
	std::vector<VkExtensionProperties>		m_vkEnabledExtensionProperties;
	std::vector<VkLayerProperties>			m_vkSelectedLayerProperties;
	std::vector<VkExtensionProperties>		m_vkSelectedExtensionProperties;
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
