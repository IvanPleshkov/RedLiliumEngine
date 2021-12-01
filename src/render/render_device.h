#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include <glm/vec2.hpp>
#include <optional>
#include <vector>
#include <memory>
#include <utility>
#include <string>

class RenderInstance;

class RenderDevice final
{
public:
    RenderDevice(const std::shared_ptr<RenderInstance>& renderInstance);
    
    ~RenderDevice();
    
    void startFrame();
    
    void endFrame(VkSemaphore waitSemaphore);
    
    VkAllocationCallbacks* allocator();

    VkDevice getVkDevice() const;
    
    std::pair<VkQueue, uint32_t> getGraphicsVkQueue() const;
    
    std::pair<VkQueue, uint32_t> getPresentationVkQueue() const;

    VkFormat getSwapChainVkImageFormat() const;

    glm::ivec2 getSwapChainSize() const;
    
    const std::vector<VkImageView>& getSwapChainVkImageViews() const;
    
    VkSemaphore getSwapChainVkSemaphore() const;
    
    uint32_t getSwapChainCurrentImageIndex() const;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    float getMaxAnisotropy() const;

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

    void initSwapChainSemaphore();

    std::shared_ptr<RenderInstance> _renderInstance;
    VkPhysicalDevice _vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice _vkDevice = VK_NULL_HANDLE;
    VkSwapchainKHR _vkSwapChain = VK_NULL_HANDLE;
    std::vector<VkImage> _swapChainVkImages;
    std::vector<VkImageView> _swapChainVkImageViews;
    uint32_t _swapChainCurrentImageIndex = 0;
    VkFormat _swapChainVkImageFormat;
    VkExtent2D _swapChainVkExtent = { 0, 0 };
    VkSemaphore _swapChainVkSemaphore = VK_NULL_HANDLE;
    std::pair<VkQueue, uint32_t> _vkGraphicsQueue;
    std::pair<VkQueue, uint32_t> _vkPresentQueue;
    float _maxAnisotropy = 1.0f;
};
