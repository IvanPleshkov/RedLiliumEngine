#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <string_view>

class RenderDevice;
class GpuBuffer;

class GpuTexture
{
public:
    GpuTexture(const std::shared_ptr<RenderDevice>& renderDevice);

    ~GpuTexture();

    void upload(std::string_view textureData);
    
    VkImageView getVkImageView() const;

    VkSampler getVkSampler() const;

    static void transitionImageLayout(const std::shared_ptr<RenderDevice>& renderDevice, VkImage vkImage, VkFormat vkFormat, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout);
    
private:
    void destroy();
    
    void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
    
    void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
    
    void createImageView();
    
    void createSampler();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkImage _vkImage = VK_NULL_HANDLE;
    VkDeviceMemory _vkImageMemory = VK_NULL_HANDLE;
    VkImageView _vkImageView = VK_NULL_HANDLE;
    VkSampler _vkSampler = VK_NULL_HANDLE;
};
