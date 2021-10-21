#include "render_device.h"
#include "render_instance.h"
#include <spdlog/spdlog.h>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>

RenderDevice::RenderDevice(RenderInstance& renderInstance)
    : _renderInstance(renderInstance)
{
    init();
}

RenderDevice::~RenderDevice()
{
    destroy();
}

RenderInstance& RenderDevice::getRenderInstance()
{
    return _renderInstance;
}


VkDevice RenderDevice::getVkDevice() const
{
    return _vkDevice;
}

VkQueue RenderDevice::getGraphicsVkQueue() const
{
    return _vkGraphicsQueue;
}

VkQueue RenderDevice::getPresentationVkQueue() const
{
    return _vkPresentQueue;
}

void RenderDevice::init()
{
    pickPhysicalDevice();
    createLogicalDevice();
}

void RenderDevice::destroy()
{
    if (_vkDevice != VK_NULL_HANDLE)
    {
        vkDestroyDevice(_vkDevice, _renderInstance.allocator());
        _vkDevice = VK_NULL_HANDLE;
        _vkPhysicalDevice = VK_NULL_HANDLE;
        _vkGraphicsQueue = VK_NULL_HANDLE;
    }
}

void RenderDevice::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_renderInstance.getVkInstance(), &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_renderInstance.getVkInstance(), &deviceCount, devices.data());
    
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices)
    {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0)
    {
        _vkPhysicalDevice = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool RenderDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::string& extensionName)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    for (const auto& extension : availableExtensions)
    {
        if (extension.extensionName == extensionName)
        {
            return true;
        }
    }
    return false;
}

int RenderDevice::rateDeviceSuitability(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);
    if (!indices.isComplete())
    {
        return 0;
    }
    
    if (!checkDeviceExtensionSupport(device, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
    {
        return 0;
    }
    
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    int score = 1;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    
    return score;
}

RenderDevice::QueueFamilyIndices RenderDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _renderInstance.getVkSurface(), &presentSupport);
        if (presentSupport) {
            indices._presentFamily = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices._graphicsFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }
        i++;
    }
    
    return indices;
}

void RenderDevice::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(_vkPhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices._graphicsFamily.value(),
        indices._presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(_vkPhysicalDevice, &createInfo, _renderInstance.allocator(), &_vkDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    vkGetDeviceQueue(_vkDevice, indices._graphicsFamily.value(), 0, &_vkGraphicsQueue);
    vkGetDeviceQueue(_vkDevice, indices._presentFamily.value(), 0, &_vkPresentQueue);
}
