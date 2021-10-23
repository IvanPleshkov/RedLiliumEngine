#include "render_pass.h"
#include "render_device.h"
#include "render_instance.h"

RenderPass::RenderPass(RenderDevice& renderDevice, VkFormat vkFormat, glm::ivec2 size)
    : _renderDevice(renderDevice)
    , _size(size)
{
    init(vkFormat);
}

RenderPass::~RenderPass()
{
    destroy();
}

VkRenderPass RenderPass::getVkRenderPass() const
{
    return _vkRenderPass;
}

glm::ivec2 RenderPass::getSize() const
{
    return _size;
}

void RenderPass::init(VkFormat vkFormat)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vkFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(_renderDevice.getVkDevice(), &renderPassInfo, _renderDevice.allocator(), &_vkRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass!");
    }
}

void RenderPass::destroy()
{
    if (_vkRenderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(_renderDevice.getVkDevice(), _vkRenderPass, _renderDevice.allocator());
        _vkRenderPass = VK_NULL_HANDLE;
    }
}
