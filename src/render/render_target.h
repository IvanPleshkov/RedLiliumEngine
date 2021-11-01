#pragma once

#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>
#include <memory>
#include <vector>

class RenderDevice;

class RenderTarget
{
public:
    RenderTarget(const std::shared_ptr<RenderDevice>& renderDevice, const std::vector<VkImageView>& vkImageViews, VkFormat vkFormat, glm::ivec2 size);

    ~RenderTarget();
    
    void bind(VkCommandBuffer vkCommandBuffer) const;
    
    void unbind(VkCommandBuffer vkCommandBuffer) const;
    
    void setFramebufferIndex(uint32_t framebufferIndex);

    VkRenderPass getVkRenderPass() const;
    
    VkSemaphore getVkSemaphore() const;

    glm::ivec2 getSize() const;

private:
    void init(VkFormat vkFormat);

    void initFramebuffer();
    
    void initSemaphore();

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    std::vector<VkFramebuffer> _vkFramebuffers;
    std::vector<VkImageView> _vkImageViews;
    uint32_t _framebufferIndex = 0;
    VkRenderPass _vkRenderPass = VK_NULL_HANDLE;
    VkSemaphore _vkSemaphore = VK_NULL_HANDLE;
    VkFormat _vkFormat;
    glm::ivec2 _size = { 0, 0 };
};
