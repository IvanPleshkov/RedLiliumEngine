#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"
#include "render_descriptor.h"
#include "render_pipeline_builder.h"
#include <stdexcept>

RenderPipeline::RenderPipeline(
                               const std::shared_ptr<RenderDevice>& renderDevice,
                               const std::shared_ptr<RenderDescriptor>& renderDescriptor,
                               const RenderPipelineBuilder& renderPipelineBuilder)
    : _renderDevice(renderDevice)
    , _renderDescriptor(renderDescriptor)
{
    init(renderPipelineBuilder);
}

RenderPipeline::~RenderPipeline()
{
    destroy();
}

void RenderPipeline::bind(VkCommandBuffer vkCommandBuffer) const
{
    vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipeline);
    if (_renderDescriptor != nullptr)
    {
        _renderDescriptor->bind(vkCommandBuffer, _vkPipelineLayout);
    }
}

VkPipelineLayout RenderPipeline::getVkPipelineLayout() const
{
    return _vkPipelineLayout;
}

VkPipeline RenderPipeline::getVkPipeline() const
{
    return _vkPipeline;
}

void RenderPipeline::init(const RenderPipelineBuilder& renderPipelineBuilder)
{
    if (vkCreatePipelineLayout(_renderDevice->getVkDevice(), &renderPipelineBuilder._vkPipelineLayoutCreateInfo, _renderDevice->allocator(), &_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    auto vkGraphicsPipelineCreateInfo = renderPipelineBuilder._vkGraphicsPipelineCreateInfo;
    vkGraphicsPipelineCreateInfo.layout = _vkPipelineLayout;
    if (vkCreateGraphicsPipelines(_renderDevice->getVkDevice(), VK_NULL_HANDLE, 1, &vkGraphicsPipelineCreateInfo, _renderDevice->allocator(), &_vkPipeline) != VK_SUCCESS)
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
    _renderDescriptor = nullptr;
}
