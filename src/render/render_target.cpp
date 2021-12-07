#include "render_target.h"
#include "render_device.h"
#include "render_instance.h"
#include "gpu_texture.h"
#include <array>
#include <stdexcept>

RenderTarget::RenderTarget(const std::shared_ptr<RenderDevice>& renderDevice, const std::vector<VkImageView>& vkImageViews, VkFormat vkFormat, glm::ivec2 size, VkSampleCountFlagBits vkSampleCountFlagBits, bool hasDepth, VkImageLayout finalColorLayout, const glm::vec4& clearColor)
    : _renderDevice(renderDevice)
    , _vkImageViews(vkImageViews)
    , _vkFormat(vkFormat)
    , _size(size)
    , _vkSampleCountFlagBits(vkSampleCountFlagBits)
    , _finalColorLayout(finalColorLayout)
    , _clearColor(clearColor)
{
    if (_vkImageViews.empty())
    {
        VkImageUsageFlags colorTextureUsage;
        if (finalColorLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            colorTextureUsage = VK_IMAGE_USAGE_SAMPLED_BIT;
        }
        else if (finalColorLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
        {
            colorTextureUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }
        _colorTexture = std::make_shared<GpuTexture>(_renderDevice, _vkFormat, VK_SAMPLE_COUNT_1_BIT, colorTextureUsage, VK_IMAGE_ASPECT_COLOR_BIT, _size, true);
        _vkImageViews.push_back(_colorTexture->getVkImageView());
    }
    if (hasDepth)
    {
        _depthTexture = std::make_shared<GpuTexture>(_renderDevice, VK_FORMAT_D32_SFLOAT, _vkSampleCountFlagBits, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, _size);
    }
    if (_vkSampleCountFlagBits != VK_SAMPLE_COUNT_1_BIT)
    {
        _msaaTexture = std::make_shared<GpuTexture>(_renderDevice, _vkFormat, _vkSampleCountFlagBits, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_ASPECT_COLOR_BIT, _size);
    }
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
    
    uint32_t clearValueCount = 1;
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{ _clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w }};
    clearValues[1].depthStencil = {1.0f, 0};
    if (_depthTexture != nullptr)
    {
        clearValueCount = 2;
    }

    renderPassInfo.clearValueCount = clearValueCount;
    renderPassInfo.pClearValues = clearValues.data();
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

bool RenderTarget::hasDepth() const
{
    return _depthTexture != nullptr;
}

VkSampleCountFlagBits RenderTarget::getVkSampleCount() const
{
    return _vkSampleCountFlagBits;
}

std::shared_ptr<GpuTexture> RenderTarget::getColorTexture()
{
    return _colorTexture;
}

std::shared_ptr<GpuTexture> RenderTarget::getDepthTexture()
{
    return _depthTexture;
}

void RenderTarget::init(VkFormat vkFormat)
{
    std::vector<VkAttachmentDescription> vkAttachmentDescriptions;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = vkFormat;
        colorAttachment.samples = _vkSampleCountFlagBits;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        if (_vkSampleCountFlagBits != VK_SAMPLE_COUNT_1_BIT)
        {
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        else
        {
            colorAttachment.finalLayout = _finalColorLayout;
        }
        vkAttachmentDescriptions.push_back(colorAttachment);
        colorAttachmentRef.attachment = static_cast<uint32_t>(vkAttachmentDescriptions.size() - 1);
    }
    
    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 0;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    if (_depthTexture != nullptr)
    {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = _vkSampleCountFlagBits;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        vkAttachmentDescriptions.push_back(depthAttachment);
        depthAttachmentRef.attachment = static_cast<uint32_t>(vkAttachmentDescriptions.size() - 1);
    }
    
    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 0;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    if (_vkSampleCountFlagBits != VK_SAMPLE_COUNT_1_BIT)
    {
        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = vkFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = _finalColorLayout;
        vkAttachmentDescriptions.push_back(colorAttachmentResolve);
        colorAttachmentResolveRef.attachment = static_cast<uint32_t>(vkAttachmentDescriptions.size() - 1);
    }

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    if (_depthTexture != nullptr)
    {
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
    }
    if (_vkSampleCountFlagBits != VK_SAMPLE_COUNT_1_BIT)
    {
        subpass.pResolveAttachments = &colorAttachmentResolveRef;
    }
    
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    if (_depthTexture != nullptr)
    {
        dependency.srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(vkAttachmentDescriptions.size());
    renderPassInfo.pAttachments = vkAttachmentDescriptions.data();
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
    _vkFramebuffers.resize(_vkImageViews.size());
    for (size_t i = 0; i < _vkImageViews.size(); i++) {
        std::vector<VkImageView> attachments;
        if (_vkSampleCountFlagBits != VK_SAMPLE_COUNT_1_BIT)
        {
            attachments.push_back(_msaaTexture->getVkImageView());
        }
        else
        {
            attachments.push_back(_vkImageViews[i]);
        }
        if (_depthTexture != nullptr)
        {
            attachments.push_back(_depthTexture->getVkImageView());
        }
        if (_vkSampleCountFlagBits != VK_SAMPLE_COUNT_1_BIT)
        {
            attachments.push_back(_vkImageViews[i]);
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
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
    _colorTexture = nullptr;
    _depthTexture = nullptr;
    _msaaTexture = nullptr;
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
