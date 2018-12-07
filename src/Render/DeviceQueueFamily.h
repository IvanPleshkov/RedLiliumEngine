#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class DeviceQueueFamily : public RedLiliumObject
{
public:
	~DeviceQueueFamily() override;

	u32 GetQueuesCount() const;
	bool SupportsRendering() const;
	bool SupportsComputeShaders() const;
	bool SupportsTransfer() const;
	bool SupportsSparseBinding() const;
	bool SupportsTimestamps() const;

RED_LILIUM_INTERNAL:
	DeviceQueueFamily(ptr<Device> device, VkQueueFamilyProperties vkQueueFamilyProperties, u32 vkIndex);
	u32 GetVkIndex() const;
	const VkQueueFamilyProperties& GetVkQueueFamilyProperties() const;

private:
	ptr<Device> m_device;
	u32 m_vkIndex;
	VkQueueFamilyProperties m_vkQueueFamilyProperties;
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
