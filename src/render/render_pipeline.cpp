#include "render_pipeline.h"
#include "render_device.h"
#include "render_instance.h"
#include <stdexcept>

RenderPipeline::RenderPipeline(
                               const std::shared_ptr<RenderDevice>& renderDevice,
                               VkDescriptorSetLayout vkDescriptorSetLayout,
                               VkPipelineLayout vkPipelineLayout,
                               VkPipeline vkPipeline)
    : _renderDevice(renderDevice)
    , _vkDescriptorSetLayout(vkDescriptorSetLayout)
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
    if (_vkDescriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(_renderDevice->getVkDevice(), _vkDescriptorSetLayout, _renderDevice->allocator());
        _vkDescriptorSetLayout = VK_NULL_HANDLE;
    }
}
