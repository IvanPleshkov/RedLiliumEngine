#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>
#include <memory>
#include <string_view>

class RenderDevice;
class GpuBuffer;
class RenderContext;

class GpuTexture : public std::enable_shared_from_this<GpuTexture>
{
public:
    GpuTexture(const std::shared_ptr<RenderDevice>& renderDevice, bool generateMips = false);

    GpuTexture(const std::shared_ptr<RenderDevice>& renderDevice, VkFormat vkFormat, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspect, glm::ivec2 size, bool needSampler = false);

    ~GpuTexture();

    void uploadStbImage(std::string_view textureData);

    glm::ivec2 getSize() const;
    
    uint32_t getMipsCount() const;
    
    VkImage getVkImage() const;
    
    VkImageView getVkImageView() const;

    VkSampler getVkSampler() const;

private:
    void destroy();

    void createImageView();
    
    void createSampler();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkImage _vkImage = VK_NULL_HANDLE;
    VkDeviceMemory _vkImageMemory = VK_NULL_HANDLE;
    VkImageView _vkImageView = VK_NULL_HANDLE;
    VkSampler _vkSampler = VK_NULL_HANDLE;
    bool _generateMips = false;
    uint32_t _mipLevels = 1;
    glm::ivec2 _size = { 0, 0 };
};
