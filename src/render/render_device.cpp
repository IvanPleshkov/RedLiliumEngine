#include "render_device.h"
#include "render_instance.h"
#include <spdlog/spdlog.h>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

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
    createSwapChain();
}

void RenderDevice::destroy()
{
    if (_vkSwapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(_vkDevice, _vkSwapChain, _renderInstance.allocator());
    }

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

bool RenderDevice::checkDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice, const std::string& extensionName)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

    for (const auto& extension : availableExtensions)
    {
        if (extension.extensionName == extensionName)
        {
            return true;
        }
    }
    return false;
}

int RenderDevice::rateDeviceSuitability(VkPhysicalDevice vkPhysicalDevice)
{
    QueueFamilyIndices indices = findQueueFamilies(vkPhysicalDevice);
    if (!indices.isComplete())
    {
        return 0;
    }
    
    if (!checkDeviceExtensionSupport(vkPhysicalDevice, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
    {
        return 0;
    }

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vkPhysicalDevice);
    if (swapChainSupport._formats.empty() || swapChainSupport._presentModes.empty())
    {
        return 0;
    }
    
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &deviceFeatures);
    
    int score = 1;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    
    return score;
}

RenderDevice::SwapChainSupportDetails RenderDevice::querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, _renderInstance.getVkSurface(), &details._capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, _renderInstance.getVkSurface(), &formatCount, nullptr);
    if (formatCount != 0) {
        details._formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, _renderInstance.getVkSurface(), &formatCount, details._formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, _renderInstance.getVkSurface(), &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details._presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, _renderInstance.getVkSurface(), &presentModeCount, details._presentModes.data());
    }
    
    return details;
}

RenderDevice::QueueFamilyIndices RenderDevice::findQueueFamilies(VkPhysicalDevice vkPhysicalDevice) {
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, _renderInstance.getVkSurface(), &presentSupport);
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

VkSurfaceFormatKHR RenderDevice::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR RenderDevice::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RenderDevice::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        SDL_GetWindowSize(_renderInstance.getSdlWindow(), &width, &height);

        VkExtent2D actualExtent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
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
    if (checkDeviceExtensionSupport(_vkPhysicalDevice, "VK_KHR_portability_subset"))
    {
        deviceExtensions.push_back("VK_KHR_portability_subset");
    }
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(_vkPhysicalDevice, &createInfo, _renderInstance.allocator(), &_vkDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    vkGetDeviceQueue(_vkDevice, indices._graphicsFamily.value(), 0, &_vkGraphicsQueue);
    vkGetDeviceQueue(_vkDevice, indices._presentFamily.value(), 0, &_vkPresentQueue);
}

void RenderDevice::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_vkPhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport._formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport._presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport._capabilities);
    
    uint32_t imageCount = swapChainSupport._capabilities.minImageCount + 1;
    if (swapChainSupport._capabilities.maxImageCount > 0 && imageCount > swapChainSupport._capabilities.maxImageCount) {
        imageCount = swapChainSupport._capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _renderInstance.getVkSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(_vkPhysicalDevice);
    uint32_t queueFamilyIndices[] =
    {
        indices._graphicsFamily.value(),
        indices._presentFamily.value()
    };

    if (indices._graphicsFamily != indices._presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    
    createInfo.preTransform = swapChainSupport._capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if (vkCreateSwapchainKHR(_vkDevice, &createInfo, _renderInstance.allocator(), &_vkSwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create swap chain!");
    }
    
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, nullptr);
    _swapChainVkImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, _swapChainVkImages.data());
    
    _swapChainVkImageFormat = surfaceFormat.format;
    _swapChainVkExtent = extent;
}
