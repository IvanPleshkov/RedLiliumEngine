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
                   const VkPipelineLayoutCreateInfo& vkPipelineLayoutCreateInfo,
                   const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo);
    
    ~RenderPipeline();

    void bind(VkCommandBuffer vkCommandBuffer) const;
    
private:
    void init(
              const VkPipelineLayoutCreateInfo& vkPipelineLayoutCreateInfo,
              const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo);

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;
    VkPipeline _vkPipeline = VK_NULL_HANDLE;
};
