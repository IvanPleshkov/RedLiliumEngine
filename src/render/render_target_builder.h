#pragma once

#include "render_pipeline.h"
#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>
#include <string_view>
#include <memory>
#include <vector>

class RenderDevice;
class RenderTarget;

class RenderTargetBuilder
{
public:
    RenderTargetBuilder(const std::shared_ptr<RenderDevice>& renderDevice);

    RenderTargetBuilder& setVkFormat(VkFormat vkFormat);
    
    RenderTargetBuilder& setSize(glm::ivec2 size);
    
    RenderTargetBuilder& addImageView(VkImageView vkImageView);
    
    RenderTargetBuilder& enableDepth(bool enable);
    
    RenderTargetBuilder& addMultisampling(VkSampleCountFlagBits vkSampleCountFlagBits);

    std::shared_ptr<RenderTarget> build();

private:
    std::shared_ptr<RenderDevice> _renderDevice;
    std::vector<VkImageView> _vkImageViews;
    VkFormat _vkFormat;
    glm::ivec2 _size;
    bool _enableDepth = false;
    VkSampleCountFlagBits _vkSampleCountFlagBits = VK_SAMPLE_COUNT_1_BIT;
};
