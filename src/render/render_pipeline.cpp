#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"
#include "render_pipeline_layout.h"
#include "render_pipeline_builder.h"
#include <stdexcept>

RenderPipeline::RenderPipeline(
                               const std::shared_ptr<RenderDevice>& renderDevice,
                               const std::shared_ptr<RenderPipelineLayout>& renderPipelineLayout,
                               const RenderPipelineBuilder& renderPipelineBuilder)
    : _renderDevice(renderDevice)
    , _renderPipelineLayout(renderPipelineLayout)
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
    _renderPipelineLayout->bind(vkCommandBuffer);
}

VkPipeline RenderPipeline::getVkPipeline() const
{
    return _vkPipeline;
}

void RenderPipeline::init(const RenderPipelineBuilder& renderPipelineBuilder)
{
    if (vkCreateGraphicsPipelines(_renderDevice->getVkDevice(), VK_NULL_HANDLE, 1, &renderPipelineBuilder._vkGraphicsPipelineCreateInfo, _renderDevice->allocator(), &_vkPipeline) != VK_SUCCESS)
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
    _renderPipelineLayout = nullptr;
}
