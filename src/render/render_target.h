#pragma once

#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>
#include <memory>
#include <vector>

class RenderDevice;
class GpuTexture;

class RenderTarget
{
public:
    RenderTarget(const std::shared_ptr<RenderDevice>& renderDevice, const std::vector<VkImageView>& vkImageViews, VkFormat vkFormat, glm::ivec2 size, VkSampleCountFlagBits vkSampleCountFlagBits, bool hasDepth);

    ~RenderTarget();
    
    void bind(VkCommandBuffer vkCommandBuffer) const;
    
    void unbind(VkCommandBuffer vkCommandBuffer) const;
    
    void setFramebufferIndex(uint32_t framebufferIndex);

    VkRenderPass getVkRenderPass() const;
    
    VkSemaphore getVkSemaphore() const;

    glm::ivec2 getSize() const;
    
    bool hasDepth() const;
    
    VkSampleCountFlagBits getVkSampleCount() const;

private:
    void init(VkFormat vkFormat);

    void initFramebuffer();
    
    void initSemaphore();
    
    void initMultisamplingResources();

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    std::vector<VkFramebuffer> _vkFramebuffers;
    std::vector<VkImageView> _vkImageViews;
    std::shared_ptr<GpuTexture> _depthTexture;
    std::shared_ptr<GpuTexture> _msaaTexture;
    uint32_t _framebufferIndex = 0;
    VkRenderPass _vkRenderPass = VK_NULL_HANDLE;
    VkSemaphore _vkSemaphore = VK_NULL_HANDLE;
    VkFormat _vkFormat;
    glm::ivec2 _size = { 0, 0 };
    VkSampleCountFlagBits _vkSampleCountFlagBits = VK_SAMPLE_COUNT_1_BIT;
};
