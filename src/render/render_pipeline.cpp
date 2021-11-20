#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"
#include "render_descriptor.h"
#include <stdexcept>

RenderPipeline::RenderPipeline(
                               const std::shared_ptr<RenderDevice>& renderDevice,
                               std::vector<std::shared_ptr<RenderDescriptor>> renderDescriptors,
                               VkPipelineLayout vkPipelineLayout,
                               VkPipeline vkPipeline)
    : _renderDevice(renderDevice)
    , _renderDescriptors(std::move(renderDescriptors))
    , _vkPipelineLayout(vkPipelineLayout)
    , _vkPipeline(vkPipeline)
{
}

RenderPipeline::~RenderPipeline()
{
    destroy();
}

void RenderPipeline::bind(VkCommandBuffer vkCommandBuffer) const
{
    vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipeline);
    for (auto& renderDescriptor : _renderDescriptors)
    {
        renderDescriptor->bind(vkCommandBuffer, _vkPipelineLayout);
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

const std::vector<std::shared_ptr<RenderDescriptor>>& RenderPipeline::getRenderDescriptors() const
{
    return _renderDescriptors;
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
    _renderDescriptors.clear();
}
