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

private:
    void destroy();
    
    void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
    
    void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);

    std::shared_ptr<RenderDevice> _renderDevice;
    VkImage _vkImage;
    VkDeviceMemory _vkImageMemory;
};
