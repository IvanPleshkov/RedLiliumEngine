#include "render_target.h"
#include "render_device.h"
#include "render_instance.h"

RenderTarget::RenderTarget(const std::shared_ptr<RenderDevice>& renderDevice, VkFormat vkFormat, glm::ivec2 size)
    : _renderDevice(renderDevice)
    , _size(size)
{
    init(vkFormat);
}

RenderTarget::~RenderTarget()
{
    destroy();
}

void RenderTarget::bind(VkCommandBuffer vkCommandBuffer) const
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _vkRenderPass;
    renderPassInfo.framebuffer = _vkFramebuffers[_framebufferIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent.width = static_cast<uint32_t>(_size.x);
    renderPassInfo.renderArea.extent.height = static_cast<uint32_t>(_size.y);
    
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(vkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderTarget::unbind(VkCommandBuffer vkCommandBuffer) const
{
    vkCmdEndRenderPass(vkCommandBuffer);
}

void RenderTarget::setFramebufferIndex(uint32_t framebufferIndex)
{
    _framebufferIndex = framebufferIndex;
}

VkRenderPass RenderTarget::getVkRenderPass() const
{
    return _vkRenderPass;
}

VkSemaphore RenderTarget::getVkSemaphore() const
{
    return _vkSemaphore;
}

glm::ivec2 RenderTarget::getSize() const
{
    return _size;
}

void RenderTarget::init(VkFormat vkFormat)
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
    
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(_renderDevice->getVkDevice(), &renderPassInfo, _renderDevice->allocator(), &_vkRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass!");
    }
    
    initFramebuffer();
    initSemaphore();
}

void RenderTarget::initFramebuffer()
{
    _vkFramebuffers.resize(_renderDevice->getSwapChainVkImageViews().size());
    for (size_t i = 0; i < _vkFramebuffers.size(); i++) {
        VkImageView attachments[] = {
            _renderDevice->getSwapChainVkImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _vkRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = static_cast<uint32_t>(_size.x);
        framebufferInfo.height = static_cast<uint32_t>(_size.y);
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(_renderDevice->getVkDevice(), &framebufferInfo, _renderDevice->allocator(), &_vkFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void RenderTarget::initSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(_renderDevice->getVkDevice(), &semaphoreInfo, _renderDevice->allocator(), &_vkSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create semaphore!");
    }
}

void RenderTarget::destroy()
{
    if (_vkSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(_renderDevice->getVkDevice(), _vkSemaphore, _renderDevice->allocator());
    }
    for (auto framebuffer : _vkFramebuffers)
    {
        vkDestroyFramebuffer(_renderDevice->getVkDevice(), framebuffer, _renderDevice->allocator());
    }
    if (_vkRenderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(_renderDevice->getVkDevice(), _vkRenderPass, _renderDevice->allocator());
        _vkRenderPass = VK_NULL_HANDLE;
    }
}
