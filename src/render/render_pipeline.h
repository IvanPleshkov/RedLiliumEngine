#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <memory>

class RenderDevice;

class RenderPipeline
{
public:
    RenderPipeline(
                   const std::shared_ptr<RenderDevice>& renderDevice,
                   VkDescriptorSetLayout vkDescriptorSetLayout,
                   VkPipelineLayout vkPipelineLayout,
                   VkPipeline vkPipeline);

    ~RenderPipeline();

    void bind(VkCommandBuffer vkCommandBuffer) const;
    
private:
    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkDescriptorSetLayout _vkDescriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;
    VkPipeline _vkPipeline = VK_NULL_HANDLE;
};
