#pragma once

#include <vulkan/vulkan.h>
#include <string>

class RenderDevice;

class RenderPipeline
{
public:
    RenderPipeline(
                   RenderDevice& renderDevice,
                   const VkPipelineLayoutCreateInfo& vkPipelineLayoutCreateInfo,
                   const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo);
    
    ~RenderPipeline();
    
private:
    void init(
              const VkPipelineLayoutCreateInfo& vkPipelineLayoutCreateInfo,
              const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo);

    void destroy();

    RenderDevice& _renderDevice;
    VkPipelineLayout _vkPipelineLayout = VK_NULL_HANDLE;
    VkPipeline _vkPipeline = VK_NULL_HANDLE;
};
