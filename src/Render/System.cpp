#include "pch.h"
#include "System.h"
#include "Device.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::Render;

System::System()
{
	VkInstanceCreateInfo instanceCreateInfo;
	MemsetZero(&instanceCreateInfo);
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	// VKTODO: set application info
	instanceCreateInfo.pApplicationInfo = nullptr; 

	// VKTODO: debug layers
	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;

	// VKTODO: investigate extensions
	instanceCreateInfo.enabledExtensionCount = 0;
	instanceCreateInfo.ppEnabledExtensionNames = nullptr;

	auto result = vkCreateInstance(&instanceCreateInfo, GetAllocator(), &m_vkInstance);
	RED_LILIUM_ASSERT(result == VK_SUCCESS);

	InitDevices();
}

System::~System()
{
	m_devices.clear();
	vkDestroyInstance(m_vkInstance, GetAllocator());
}

void System::InitDevices()
{}

VkAllocationCallbacks* System::GetAllocator()
{
	return nullptr;
}
