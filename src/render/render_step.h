#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class RenderDevice;
class RenderTarget;
class RenderPipeline;

class RenderStep
{
public:
    RenderStep(const std::shared_ptr<RenderDevice>& renderDevice,
               const std::shared_ptr<RenderTarget>& renderTarget,
               const std::shared_ptr<RenderPipeline>& renderPipeline);

    ~RenderStep();

    void draw(VkSemaphore waitVkSemaphore);
    
public:
    void init();

    void destroy();
    
    void buildCommandBuffer();

    void destroyCommandBuffer();
    
    void runCommandBuffer(VkSemaphore waitVkSemaphore);

    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<RenderTarget> _renderTarget;
    std::shared_ptr<RenderPipeline> _renderPipeline;
    VkCommandPool _vkCommandPool = VK_NULL_HANDLE;
    VkCommandBuffer _vkCommandBuffer = VK_NULL_HANDLE;
};
