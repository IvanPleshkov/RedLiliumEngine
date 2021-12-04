#include "render_target.h"
#include "render_device.h"
#include "render_instance.h"
#include "gpu_texture.h"
#include <array>
#include <stdexcept>

RenderTarget::RenderTarget(const std::shared_ptr<RenderDevice>& renderDevice, const std::vector<VkImageView>& vkImageViews, VkFormat vkFormat, glm::ivec2 size, bool hasDepth)
    : _renderDevice(renderDevice)
    , _vkImageViews(vkImageViews)
    , _vkFormat(vkFormat)
    , _size(size)
{
    if (hasDepth)
    {
        initDepthResources();
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
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    if (_depthVkImage != VK_NULL_HANDLE)
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
    return _depthVkImage != VK_NULL_HANDLE;
}

void RenderTarget::init(VkFormat vkFormat)
{
    std::vector<VkAttachmentDescription> vkAttachmentDescriptions;
    
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
        vkAttachmentDescriptions.push_back(colorAttachment);
    }
    
    if (_depthVkImage != VK_NULL_HANDLE)
    {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        vkAttachmentDescriptions.push_back(depthAttachment);
    }

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    if (_depthVkImage != VK_NULL_HANDLE)
    {
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
    }
    
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    if (_depthVkImage != VK_NULL_HANDLE)
    {
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    else
    {
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
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
        attachments.push_back(_vkImageViews[i]);
        if (_depthVkImageView != VK_NULL_HANDLE)
        {
            attachments.push_back(_depthVkImageView);
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

void RenderTarget::initDepthResources()
{
    VkImageCreateInfo vkImageCreateInfo{};
    vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    vkImageCreateInfo.format = VK_FORMAT_D32_SFLOAT;
    vkImageCreateInfo.extent.width = _size.x;
    vkImageCreateInfo.extent.height = _size.y;
    vkImageCreateInfo.extent.depth = 1;
    vkImageCreateInfo.mipLevels = 1;
    vkImageCreateInfo.arrayLayers = 1;
    vkImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    vkImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    vkImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    vkImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkImageCreateInfo.queueFamilyIndexCount = 0;
    vkImageCreateInfo.pQueueFamilyIndices = nullptr;
    vkImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (vkCreateImage(_renderDevice->getVkDevice(), &vkImageCreateInfo, _renderDevice->allocator(), &_depthVkImage) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements vkMemoryRequirements;
    vkGetImageMemoryRequirements(_renderDevice->getVkDevice(), _depthVkImage, &vkMemoryRequirements);

    VkMemoryAllocateInfo vkMemoryAllocateInfo{};
    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = _renderDevice->findMemoryType(vkMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(_renderDevice->getVkDevice(), &vkMemoryAllocateInfo, _renderDevice->allocator(), &_depthVkDeviceMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(_renderDevice->getVkDevice(), _depthVkImage, _depthVkDeviceMemory, 0);

    VkImageViewCreateInfo vkImageViewCreateInfo{};
    vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vkImageViewCreateInfo.image = _depthVkImage;
    vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vkImageViewCreateInfo.format = vkImageCreateInfo.format;
    vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    vkImageViewCreateInfo.subresourceRange.levelCount = 1;
    vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    vkImageViewCreateInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(_renderDevice->getVkDevice(), &vkImageViewCreateInfo, _renderDevice->allocator(), &_depthVkImageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    // GpuTexture::transitionImageLayout(_renderDevice, _depthVkImage, vkImageCreateInfo.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
}

void RenderTarget::destroy()
{
    if (_vkSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(_renderDevice->getVkDevice(), _vkSemaphore, _renderDevice->allocator());
    }
    if (_depthVkImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(_renderDevice->getVkDevice(), _depthVkImageView, _renderDevice->allocator());
        _depthVkImageView = VK_NULL_HANDLE;
    }
    if (_depthVkImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(_renderDevice->getVkDevice(), _depthVkImage, _renderDevice->allocator());
        _depthVkImage = VK_NULL_HANDLE;
    }
    if (_depthVkDeviceMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(_renderDevice->getVkDevice(), _depthVkDeviceMemory, _renderDevice->allocator());
        _depthVkDeviceMemory = VK_NULL_HANDLE;
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
