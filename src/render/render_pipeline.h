#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include <vector>

class RenderDevice;
class RenderDescriptor;

class RenderPipeline
{
public:
    RenderPipeline(
                   const std::shared_ptr<RenderDevice>& renderDevice,
                   std::vector<std::shared_ptr<RenderDescriptor>> renderDescriptors,
                   VkPipelineLayout vkPipelineLayout,
                   VkPipeline vkPipeline);

    ~RenderPipeline();

    void bind(VkCommandBuffer vkCommandBuffer) const;

    VkPipelineLayout getVkPipelineLayout() const;

    VkPipeline getVkPipeline() const;

    const std::vector<std::shared_ptr<RenderDescriptor>>& getRenderDescriptors() const;

private:
    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    std::vector<std::shared_ptr<RenderDescriptor>> _renderDescriptors;
    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;
    VkPipeline _vkPipeline = VK_NULL_HANDLE;
};
