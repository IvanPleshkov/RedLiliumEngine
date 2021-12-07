#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class RenderDevice;
class GpuTexture;

class RenderTarget
{
public:
    RenderTarget(const std::shared_ptr<RenderDevice>& renderDevice, const std::vector<VkImageView>& vkImageViews, VkFormat vkFormat, glm::ivec2 size, VkSampleCountFlagBits vkSampleCountFlagBits, bool hasDepth, VkImageLayout finalColorLayout, const glm::vec4& clearColor);

    ~RenderTarget();
    
    void bind(VkCommandBuffer vkCommandBuffer) const;
    
    void unbind(VkCommandBuffer vkCommandBuffer) const;
    
    void setFramebufferIndex(uint32_t framebufferIndex);

    VkRenderPass getVkRenderPass() const;
    
    VkSemaphore getVkSemaphore() const;

    glm::ivec2 getSize() const;
    
    bool hasDepth() const;
    
    VkSampleCountFlagBits getVkSampleCount() const;
    
    std::shared_ptr<GpuTexture> getColorTexture();

    std::shared_ptr<GpuTexture> getDepthTexture();

private:
    void init(VkFormat vkFormat);

    void initFramebuffer();
    
    void initSemaphore();
    
    void initMultisamplingResources();

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    std::vector<VkFramebuffer> _vkFramebuffers;
    std::vector<VkImageView> _vkImageViews;
    std::shared_ptr<GpuTexture> _colorTexture;
    std::shared_ptr<GpuTexture> _depthTexture;
    std::shared_ptr<GpuTexture> _msaaTexture;
    uint32_t _framebufferIndex = 0;
    VkRenderPass _vkRenderPass = VK_NULL_HANDLE;
    VkSemaphore _vkSemaphore = VK_NULL_HANDLE;
    VkFormat _vkFormat;
    glm::ivec2 _size = { 0, 0 };
    glm::vec4 _clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    VkSampleCountFlagBits _vkSampleCountFlagBits = VK_SAMPLE_COUNT_1_BIT;
    VkImageLayout _finalColorLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
};
