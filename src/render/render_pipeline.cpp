#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"

RenderPipeline::RenderPipeline(
                               const std::shared_ptr<RenderDevice>& renderDevice,
                               const VkPipelineLayoutCreateInfo& vkPipelineLayoutCreateInfo,
                               const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo)
    : _renderDevice(renderDevice)
{
    init(vkPipelineLayoutCreateInfo, vkGraphicsPipelineCreateInfo);
}

RenderPipeline::~RenderPipeline()
{
    destroy();
}

void RenderPipeline::bind(VkCommandBuffer vkCommandBuffer) const
{
    vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipeline);
}

void RenderPipeline::init(
                          const VkPipelineLayoutCreateInfo& vkPipelineLayoutCreateInfo,
                          const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo)
{
    if (vkCreatePipelineLayout(_renderDevice->getVkDevice(), &vkPipelineLayoutCreateInfo, _renderDevice->allocator(), &_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = vkGraphicsPipelineCreateInfo;
    pipelineCreateInfo.layout = _vkPipelineLayout;
    if (vkCreateGraphicsPipelines(_renderDevice->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, _renderDevice->allocator(), &_vkPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create graphics pipeline!");
    }
}

void RenderPipeline::destroy()
{
    if (_vkPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(_renderDevice->getVkDevice(), _vkPipeline, _renderDevice->allocator());
        _vkPipeline = VK_NULL_HANDLE;
    }
    if (_vkPipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(_renderDevice->getVkDevice(), _vkPipelineLayout, _renderDevice->allocator());
        _vkPipelineLayout = VK_NULL_HANDLE;
    }
}
