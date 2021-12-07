#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

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

    RenderTargetBuilder& setFinalColorLayout(VkImageLayout finalColorLayout);
    
    RenderTargetBuilder& setClearColor(const glm::vec4& clearColor);

    std::shared_ptr<RenderTarget> build();

private:
    std::shared_ptr<RenderDevice> _renderDevice;
    std::vector<VkImageView> _vkImageViews;
    VkFormat _vkFormat = VK_FORMAT_R8G8B8A8_SNORM;
    glm::ivec2 _size;
    glm::vec4 _clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    bool _enableDepth = false;
    VkSampleCountFlagBits _vkSampleCountFlagBits = VK_SAMPLE_COUNT_1_BIT;
    VkImageLayout _finalColorLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
};
