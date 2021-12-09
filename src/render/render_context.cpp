#include "render_context.h"
#include "render_target.h"
#include "render_pipeline.h"
#include "render_device.h"
#include "gpu_mesh.h"
#include "gpu_buffer.h"
#include "gpu_texture.h"
#include "render_pipeline_layout.h"
#include <stdexcept>

RenderContext::RenderContext(const std::shared_ptr<RenderDevice>& renderDevice, VkQueue vkQueue, uint32_t vkFamilyIndex)
    : _renderDevice(renderDevice)
    , _vkQueue(vkQueue)
    , _vkFamilyIndex(vkFamilyIndex)
{
    init();
}

RenderContext::~RenderContext()
{
    destroy();
}

void RenderContext::draw(
          const std::shared_ptr<RenderTarget>& renderTarget,
          const std::shared_ptr<RenderPipeline>& renderPipeline,
          const std::shared_ptr<GpuMesh>& gpuMesh)
{
    if (_vkCommandBuffer == VK_NULL_HANDLE)
    {
        initCommandBuffer();
    }

    renderTarget->bind(_vkCommandBuffer);
    renderPipeline->bind(_vkCommandBuffer);

    if (gpuMesh != nullptr)
    {
        VkBuffer vertexBuffers[] = { gpuMesh->getVertexBuffer()->getVkBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(_vkCommandBuffer, 0, 1, vertexBuffers, offsets);

        if (gpuMesh->getIndexBuffer() != nullptr)
        {
            auto vkIndexBuffer = gpuMesh->getIndexBuffer()->getVkBuffer();
            vkCmdBindIndexBuffer(_vkCommandBuffer, vkIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(_vkCommandBuffer, static_cast<uint32_t>(gpuMesh->getIndicesCount()), 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(_vkCommandBuffer, 3, 1, 0, 0);
        }
    }
    else
    {
        vkCmdDraw(_vkCommandBuffer, 3, 1, 0, 0);
    }
    renderTarget->unbind(_vkCommandBuffer);
}

void RenderContext::copyBufferToImage(const std::shared_ptr<GpuBuffer>& gpuBuffer, const std::shared_ptr<GpuTexture>& gpuTexture, uint32_t mipLevel)
{
    if (_vkCommandBuffer == VK_NULL_HANDLE)
    {
        initCommandBuffer();
    }

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = mipLevel;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        static_cast<uint32_t>(gpuTexture->getSize().x),
        static_cast<uint32_t>(gpuTexture->getSize().y),
        1
    };

    vkCmdCopyBufferToImage(
        _vkCommandBuffer,
        gpuBuffer->getVkBuffer(),
        gpuTexture->getVkImage(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );
}

void RenderContext::copyImageToBuffer(const std::shared_ptr<GpuTexture>& gpuTexture, const std::shared_ptr<GpuBuffer>& gpuBuffer, uint32_t mipLevel)
{
    if (_vkCommandBuffer == VK_NULL_HANDLE)
    {
        initCommandBuffer();
    }

    VkBufferImageCopy vkBufferImageCopy{};
    vkBufferImageCopy.bufferOffset = 0;
    vkBufferImageCopy.bufferRowLength = 0;
    vkBufferImageCopy.bufferImageHeight = 0;

    vkBufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vkBufferImageCopy.imageSubresource.mipLevel = mipLevel;
    vkBufferImageCopy.imageSubresource.baseArrayLayer = 0;
    vkBufferImageCopy.imageSubresource.layerCount = 1;

    vkBufferImageCopy.imageOffset = {0, 0, 0};
    vkBufferImageCopy.imageExtent = {
        static_cast<uint32_t>(gpuTexture->getSize().x),
        static_cast<uint32_t>(gpuTexture->getSize().y),
        1
    };

    vkCmdCopyImageToBuffer(
        _vkCommandBuffer,
        gpuTexture->getVkImage(),
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        gpuBuffer->getVkBuffer(),
        1,
        &vkBufferImageCopy
    );
}

void RenderContext::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    if (_vkCommandBuffer == VK_NULL_HANDLE)
    {
        initCommandBuffer();
    }

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(_vkCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
}

void RenderContext::transitionImageLayout(const std::shared_ptr<GpuTexture>& gpuTexture, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, uint32_t mipsLevel)
{
    if (_vkCommandBuffer == VK_NULL_HANDLE)
    {
        initCommandBuffer();
    }

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldVkImageLayout;
    barrier.newLayout = newVkImageLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = gpuTexture->getVkImage();
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
        _vkCommandBuffer,
        sourceStage,
        destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}

void RenderContext::generateMipmaps(const std::shared_ptr<GpuTexture>& gpuTexture)
{
    if (_vkCommandBuffer == VK_NULL_HANDLE)
    {
        initCommandBuffer();
    }

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = gpuTexture->getVkImage();
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = gpuTexture->getSize().x;
    int32_t mipHeight = gpuTexture->getSize().y;

    for (uint32_t i = 1; i < gpuTexture->getMipsCount(); i++)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(_vkCommandBuffer,
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
    
        vkCmdBlitImage(_vkCommandBuffer,
            gpuTexture->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            gpuTexture->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);
    
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(_vkCommandBuffer,
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

    barrier.subresourceRange.baseMipLevel = gpuTexture->getMipsCount() - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(_vkCommandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);
}

void RenderContext::run(VkSemaphore waitVkSemaphore, VkSemaphore signalVkSemaphore)
{
    if (vkEndCommandBuffer(_vkCommandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_vkCommandBuffer;

    VkSemaphore waitSemaphores[] = { waitVkSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    if (waitVkSemaphore != VK_NULL_HANDLE)
    {
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
    }
    
    VkSemaphore signalSemaphores[] = { signalVkSemaphore };
    if (signalVkSemaphore != VK_NULL_HANDLE)
    {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
    }
    
    if (vkQueueSubmit(_vkQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    vkDeviceWaitIdle(_renderDevice->getVkDevice());
    destroyCommandBuffer();
}

void RenderContext::init()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = _vkFamilyIndex;
    poolInfo.flags = 0;
    
    if (vkCreateCommandPool(_renderDevice->getVkDevice(), &poolInfo, _renderDevice->allocator(), &_vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
    
    initCommandBuffer();
}

void RenderContext::initCommandBuffer()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(_renderDevice->getVkDevice(), &allocInfo, &_vkCommandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(_vkCommandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

void RenderContext::destroy()
{
    destroyCommandBuffer();
    if (_vkCommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(_renderDevice->getVkDevice(), _vkCommandPool, _renderDevice->allocator());
        _vkCommandPool = VK_NULL_HANDLE;
    }
}

void RenderContext::destroyCommandBuffer()
{
    if (_vkCommandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(_renderDevice->getVkDevice(), _vkCommandPool, 1, &_vkCommandBuffer);
        _vkCommandBuffer = VK_NULL_HANDLE;
    }
}
