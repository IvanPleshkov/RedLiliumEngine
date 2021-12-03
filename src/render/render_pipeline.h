#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include <vector>

class RenderDevice;
class RenderDescriptor;
class RenderPipelineBuilder;

class RenderPipeline
{
public:
    RenderPipeline(
                   const std::shared_ptr<RenderDevice>& renderDevice,
                   const std::shared_ptr<RenderDescriptor>& renderDescriptor,
                   const RenderPipelineBuilder& renderPipelineBuilder);

    ~RenderPipeline();

    void bind(VkCommandBuffer vkCommandBuffer) const;

    VkPipeline getVkPipeline() const;

private:
    void init(const RenderPipelineBuilder& renderPipelineBuilder);

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<RenderDescriptor> _renderDescriptor;
    VkPipeline _vkPipeline = VK_NULL_HANDLE;
};
