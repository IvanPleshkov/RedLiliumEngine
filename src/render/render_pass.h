#pragma once

#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>

class RenderDevice;

class RenderPass
{
public:
    RenderPass(RenderDevice& renderDevice, VkFormat vkFormat, glm::ivec2 size);
    
    ~RenderPass();
    
    VkRenderPass getVkRenderPass() const;
    
    glm::ivec2 getSize() const;

private:
    void init(VkFormat vkFormat);

    void destroy();

    RenderDevice& _renderDevice;
    VkRenderPass _vkRenderPass;
    glm::ivec2 _size;
};
