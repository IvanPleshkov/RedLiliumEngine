#include "gpu_texture.h"
#include "gpu_buffer.h"
#include "render_device.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cmath>

GpuTexture::GpuTexture(const std::shared_ptr<RenderDevice>& renderDevice, bool generateMips)
    : _renderDevice(renderDevice)
    , _generateMips(generateMips)
{ }

GpuTexture::~GpuTexture()
{
    destroy();
}

VkImageView GpuTexture::getVkImageView() const
{
    return _vkImageView;
}

VkSampler GpuTexture::getVkSampler() const
{
    return _vkSampler;
}

void GpuTexture::upload(std::string_view textureData)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(textureData.data()), static_cast<int>(textureData.size()), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    if (!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }
    if (_generateMips)
    {
        _mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    }
    else
    {
        _mipLevels = 1;
    }

    auto stagingBuffer = std::make_shared<GpuBuffer>(_renderDevice, GpuBuffer::Uniform, false);
    stagingBuffer->update(reinterpret_cast<const char*>(pixels), texWidth * texHeight * 4);
    stbi_image_free(pixels);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = _mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    if (_generateMips)
    {
        imageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    if (vkCreateImage(_renderDevice->getVkDevice(), &imageInfo, _renderDevice->allocator(), &_vkImage) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_renderDevice->getVkDevice(), _vkImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _renderDevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(_renderDevice->getVkDevice(), &allocInfo, _renderDevice->allocator(), &_vkImageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(_renderDevice->getVkDevice(), _vkImage, _vkImageMemory, 0);
    
    transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer->getVkBuffer(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

    if (_generateMips)
    {
        generateMipmaps(texWidth, texHeight);
    }
    else
    {
        transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    
    createImageView();
    createSampler();
}

void GpuTexture::destroy()
{
    if (_vkSampler != VK_NULL_HANDLE)
    {
        vkDestroySampler(_renderDevice->getVkDevice(), _vkSampler, _renderDevice->allocator());
        _vkSampler = VK_NULL_HANDLE;
    }
    if (_vkImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(_renderDevice->getVkDevice(), _vkImageView, _renderDevice->allocator());
        _vkImageView = VK_NULL_HANDLE;
    }
    if (_vkImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(_renderDevice->getVkDevice(), _vkImage, _renderDevice->allocator());
        _vkImage = VK_NULL_HANDLE;
    }
    if (_vkImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(_renderDevice->getVkDevice(), _vkImageMemory, _renderDevice->allocator());
        _vkImageMemory = VK_NULL_HANDLE;
    }
}

void GpuTexture::transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
{
    transitionImageLayout(_renderDevice, _vkImage, VK_FORMAT_R8G8B8A8_SRGB, oldLayout, newLayout, _mipLevels);
}

void GpuTexture::transitionImageLayout(const std::shared_ptr<RenderDevice>& renderDevice, VkImage vkImage, VkFormat vkFormat, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, uint32_t mipsLevel)
{
    auto graphicsQueue = renderDevice->getGraphicsVkQueue();
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueue.second;
    poolInfo.flags = 0;

    VkCommandPool vkCommandPool;
    if (vkCreateCommandPool(renderDevice->getVkDevice(), &poolInfo, renderDevice->allocator(), &vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer vkCommandBuffer;
    vkAllocateCommandBuffers(renderDevice->getVkDevice(), &allocInfo, &vkCommandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldVkImageLayout;
    barrier.newLayout = newVkImageLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = vkImage;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipsLevel;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (newVkImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    else
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldVkImageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newVkImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldVkImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newVkImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldVkImageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newVkImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        vkCommandBuffer,
        sourceStage,
        destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vkEndCommandBuffer(vkCommandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCommandBuffer;

    vkQueueSubmit(renderDevice->getGraphicsVkQueue().first, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderDevice->getGraphicsVkQueue().first);

    vkFreeCommandBuffers(renderDevice->getVkDevice(), vkCommandPool, 1, &vkCommandBuffer);
    vkDestroyCommandPool(renderDevice->getVkDevice(), vkCommandPool, renderDevice->allocator());
}

void GpuTexture::copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height)
{
    auto graphicsQueue = _renderDevice->getGraphicsVkQueue();
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueue.second;
    poolInfo.flags = 0;

    VkCommandPool vkCommandPool;
    if (vkCreateCommandPool(_renderDevice->getVkDevice(), &poolInfo, _renderDevice->allocator(), &vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer vkCommandBuffer;
    vkAllocateCommandBuffers(_renderDevice->getVkDevice(), &allocInfo, &vkCommandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        vkCommandBuffer,
        buffer,
        _vkImage,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    vkEndCommandBuffer(vkCommandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCommandBuffer;

    vkQueueSubmit(_renderDevice->getGraphicsVkQueue().first, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_renderDevice->getGraphicsVkQueue().first);
    
    vkFreeCommandBuffers(_renderDevice->getVkDevice(), vkCommandPool, 1, &vkCommandBuffer);
    vkDestroyCommandPool(_renderDevice->getVkDevice(), vkCommandPool, _renderDevice->allocator());
}

void GpuTexture::createImageView()
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = _vkImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = _mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(_renderDevice->getVkDevice(), &viewInfo, _renderDevice->allocator(), &_vkImageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void GpuTexture::createSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = _renderDevice->getMaxAnisotropy();
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    
    if (_mipLevels > 1)
    {
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(_mipLevels);
        samplerInfo.mipLodBias = 0.0f;
    }
    
    if (vkCreateSampler(_renderDevice->getVkDevice(), &samplerInfo, _renderDevice->allocator(), &_vkSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void GpuTexture::generateMipmaps(int32_t texWidth, int32_t texHeight)
{
    auto graphicsQueue = _renderDevice->getGraphicsVkQueue();
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueue.second;
    poolInfo.flags = 0;

    VkCommandPool vkCommandPool;
    if (vkCreateCommandPool(_renderDevice->getVkDevice(), &poolInfo, _renderDevice->allocator(), &vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer vkCommandBuffer;
    vkAllocateCommandBuffers(_renderDevice->getVkDevice(), &allocInfo, &vkCommandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = _vkImage;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < _mipLevels; i++)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(vkCommandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;
    
        vkCmdBlitImage(vkCommandBuffer,
            _vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            _vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);
    
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(vkCommandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        if (mipWidth > 1)
        {
            mipWidth /= 2;
        }
        if (mipHeight > 1)
        {
            mipHeight /= 2;
        }
    }

    barrier.subresourceRange.baseMipLevel = _mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(vkCommandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    vkEndCommandBuffer(vkCommandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCommandBuffer;

    vkQueueSubmit(_renderDevice->getGraphicsVkQueue().first, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_renderDevice->getGraphicsVkQueue().first);
    
    vkFreeCommandBuffers(_renderDevice->getVkDevice(), vkCommandPool, 1, &vkCommandBuffer);
    vkDestroyCommandPool(_renderDevice->getVkDevice(), vkCommandPool, _renderDevice->allocator());
}
