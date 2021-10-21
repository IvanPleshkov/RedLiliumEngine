#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include <optional>
#include <vector>

class RenderInstance;

class RenderDevice final
{
public:
    RenderDevice(RenderInstance& renderInstance);
    
    ~RenderDevice();
    
    RenderInstance& getRenderInstance();
    
    VkDevice getVkDevice() const;
    
    VkQueue getGraphicsVkQueue() const;
    
    VkQueue getPresentationVkQueue() const;

private:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR _capabilities;
        std::vector<VkSurfaceFormatKHR> _formats;
        std::vector<VkPresentModeKHR> _presentModes;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> _graphicsFamily;
        std::optional<uint32_t> _presentFamily;
        
        bool isComplete() {
            return _graphicsFamily.has_value() && _presentFamily.has_value();
        }
    };

    void init();

    void destroy();
    
    void pickPhysicalDevice();
    
    bool checkDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice, const std::string& extensionName);
    
    int rateDeviceSuitability(VkPhysicalDevice vkPhysicalDevice);
    
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice vkPhysicalDevice);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createLogicalDevice();
    
    void createSwapChain();
    
    void createImageViews();

    RenderInstance& _renderInstance;
    VkPhysicalDevice _vkPhysicalDevice;
    VkDevice _vkDevice;
    VkSwapchainKHR _vkSwapChain;
    std::vector<VkImage> _swapChainVkImages;
    std::vector<VkImageView> _swapChainVkImageViews;
    VkFormat _swapChainVkImageFormat;
    VkExtent2D _swapChainVkExtent;
    VkQueue _vkGraphicsQueue;
    VkQueue _vkPresentQueue;
};
