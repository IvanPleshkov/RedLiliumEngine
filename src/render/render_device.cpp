#include "render_device.h"
#include "render_instance.h"
#include <spdlog/spdlog.h>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

RenderDevice::RenderDevice(const std::shared_ptr<RenderInstance>& renderInstance)
    : _renderInstance(renderInstance)
{
    init();
}

RenderDevice::~RenderDevice()
{
    destroy();
}

void RenderDevice::startFrame()
{
    vkAcquireNextImageKHR(_vkDevice, _vkSwapChain, UINT64_MAX, _swapChainVkSemaphore, VK_NULL_HANDLE, &_swapChainCurrentImageIndex);
}

void RenderDevice::endFrame(VkSemaphore waitSemaphore)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &waitSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &_vkSwapChain;
    presentInfo.pImageIndices = &_swapChainCurrentImageIndex;
    presentInfo.pResults = nullptr;
    vkQueuePresentKHR(_vkPresentQueue.first, &presentInfo);
    
    vkDeviceWaitIdle(_vkDevice);
}

VkAllocationCallbacks* RenderDevice::allocator()
{
    return _renderInstance->allocator();
}

VkDevice RenderDevice::getVkDevice() const
{
    return _vkDevice;
}

std::pair<VkQueue, uint32_t> RenderDevice::getGraphicsVkQueue() const
{
    return _vkGraphicsQueue;
}

std::pair<VkQueue, uint32_t> RenderDevice::getPresentationVkQueue() const
{
    return _vkPresentQueue;
}

VkFormat RenderDevice::getSwapChainVkImageFormat() const
{
    return _swapChainVkImageFormat;
}

glm::ivec2 RenderDevice::getSwapChainSize() const
{
    return {
        static_cast<int>(_swapChainVkExtent.width),
        static_cast<int>(_swapChainVkExtent.height)};
}

const std::vector<VkImageView>& RenderDevice::getSwapChainVkImageViews() const
{
    return _swapChainVkImageViews;
}

VkSemaphore RenderDevice::getSwapChainVkSemaphore() const
{
    return _swapChainVkSemaphore;
}

uint32_t RenderDevice::getSwapChainCurrentImageIndex() const
{
    return _swapChainCurrentImageIndex;
}

uint32_t RenderDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

float RenderDevice::getMaxAnisotropy() const
{
    return _maxAnisotropy;
}

void RenderDevice::init()
{
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    initSwapChainSemaphore();
}

void RenderDevice::destroy()
{
    if (_swapChainVkSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(_vkDevice, _swapChainVkSemaphore, allocator());
        _swapChainVkSemaphore = VK_NULL_HANDLE;
    }
    for (auto imageView : _swapChainVkImageViews)
    {
        vkDestroyImageView(_vkDevice, imageView, _renderInstance->allocator());
    }

    if (_vkSwapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(_vkDevice, _vkSwapChain, _renderInstance->allocator());
    }

    if (_vkDevice != VK_NULL_HANDLE)
    {
        vkDestroyDevice(_vkDevice, _renderInstance->allocator());
        _vkDevice = VK_NULL_HANDLE;
        _vkPhysicalDevice = VK_NULL_HANDLE;
        _vkGraphicsQueue = { VK_NULL_HANDLE, 0 };
        _vkPresentQueue = { VK_NULL_HANDLE, 0 };
    }
}

void RenderDevice::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_renderInstance->getVkInstance(), &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_renderInstance->getVkInstance(), &deviceCount, devices.data());
    
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
    
    if (!deviceFeatures.samplerAnisotropy)
    {
        return 0;
    }
    
    int score = 1;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    
    return score;
}

RenderDevice::SwapChainSupportDetails RenderDevice::querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, _renderInstance->getVkSurface(), &details._capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, _renderInstance->getVkSurface(), &formatCount, nullptr);
    if (formatCount != 0) {
        details._formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, _renderInstance->getVkSurface(), &formatCount, details._formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, _renderInstance->getVkSurface(), &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details._presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, _renderInstance->getVkSurface(), &presentModeCount, details._presentModes.data());
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
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, _renderInstance->getVkSurface(), &presentSupport);
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
        SDL_GetWindowSize(_renderInstance->getSdlWindow(), &width, &height);

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

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(_vkPhysicalDevice, &physicalDeviceProperties);
    // VkPhysicalDeviceFeatures physicalDeviceFeatures;
    // vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &physicalDeviceFeatures);
    _maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy;
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

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

    if (vkCreateDevice(_vkPhysicalDevice, &createInfo, _renderInstance->allocator(), &_vkDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    vkGetDeviceQueue(_vkDevice, indices._graphicsFamily.value(), 0, &_vkGraphicsQueue.first);
    _vkGraphicsQueue.second = indices._graphicsFamily.value();
    vkGetDeviceQueue(_vkDevice, indices._presentFamily.value(), 0, &_vkPresentQueue.first);
    _vkPresentQueue.second = indices._presentFamily.value();
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
    createInfo.surface = _renderInstance->getVkSurface();
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
    
    if (vkCreateSwapchainKHR(_vkDevice, &createInfo, _renderInstance->allocator(), &_vkSwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create swap chain!");
    }
    
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, nullptr);
    _swapChainVkImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_vkDevice, _vkSwapChain, &imageCount, _swapChainVkImages.data());
    
    _swapChainVkImageFormat = surfaceFormat.format;
    _swapChainVkExtent = extent;
}

void RenderDevice::createImageViews()
{
    _swapChainVkImageViews.resize(_swapChainVkImages.size());
    for (size_t i = 0; i < _swapChainVkImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapChainVkImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapChainVkImageFormat;
        
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(_vkDevice, &createInfo, _renderInstance->allocator(), &_swapChainVkImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}

void RenderDevice::initSwapChainSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(_vkDevice, &semaphoreInfo, allocator(), &_swapChainVkSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create semaphore!");
    }
}
