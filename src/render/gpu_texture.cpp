#include "gpu_texture.h"
#include "gpu_buffer.h"
#include "render_device.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GpuTexture::GpuTexture(const std::shared_ptr<RenderDevice>& renderDevice)
    : _renderDevice(renderDevice)
{ }

GpuTexture::~GpuTexture()
{
    destroy();
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
    
    auto stagingBuffer = std::make_shared<GpuBuffer>(_renderDevice, GpuBuffer::Uniform, false);
    stagingBuffer->update(reinterpret_cast<const char*>(pixels), texWidth * texHeight * 4);
    stbi_image_free(pixels);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
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

    transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void GpuTexture::destroy()
{
    vkDestroyImage(_renderDevice->getVkDevice(), _vkImage, _renderDevice->allocator());
    vkFreeMemory(_renderDevice->getVkDevice(), _vkImageMemory, _renderDevice->allocator());
}

void GpuTexture::transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
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
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = _vkImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
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

    vkQueueSubmit(_renderDevice->getGraphicsVkQueue().first, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_renderDevice->getGraphicsVkQueue().first);
    
    vkFreeCommandBuffers(_renderDevice->getVkDevice(), vkCommandPool, 1, &vkCommandBuffer);
    vkDestroyCommandPool(_renderDevice->getVkDevice(), vkCommandPool, _renderDevice->allocator());
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
