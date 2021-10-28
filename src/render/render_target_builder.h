#pragma once

#include "render_pipeline.h"
#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>
#include <string_view>
#include <memory>

class RenderDevice;
class RenderTarget;

class RenderTargetBuilder
{
public:
    RenderTargetBuilder(const std::shared_ptr<RenderDevice>& renderDevice);

    RenderTargetBuilder& setVkFormat(VkFormat vkFormat);
    
    RenderTargetBuilder& setSize(glm::ivec2 size);
    
    std::shared_ptr<RenderTarget> build();

private:
    std::shared_ptr<RenderDevice> _renderDevice;
    VkFormat _vkFormat;
    glm::ivec2 _size;
};
