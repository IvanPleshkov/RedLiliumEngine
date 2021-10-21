#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include <optional>

class RenderInstance;

class RenderDevice
{
public:
    struct QueueFamilyIndices {
        std::optional<uint32_t> _graphicsFamily;
        std::optional<uint32_t> _presentFamily;
        
        bool isComplete() {
            return _graphicsFamily.has_value() && _presentFamily.has_value();
        }
    };

    RenderDevice(RenderInstance& renderInstance);
    
    ~RenderDevice();
    
    RenderInstance& getRenderInstance();
    
    VkDevice getVkDevice() const;
    
    VkQueue getGraphicsVkQueue() const;
    
    VkQueue getPresentationVkQueue() const;

private:
    void init();

    void destroy();
    
    void pickPhysicalDevice();
    
    bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::string& extensionName);
    
    int rateDeviceSuitability(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void createLogicalDevice();

    RenderInstance& _renderInstance;
    VkPhysicalDevice _vkPhysicalDevice;
    VkDevice _vkDevice;
    VkQueue _vkGraphicsQueue;
    VkQueue _vkPresentQueue;
};
