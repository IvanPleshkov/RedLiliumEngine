#include "render_step.h"
#include "render_target.h"
#include "render_pipeline.h"
#include "render_device.h"
#include <stdexcept>

RenderStep::RenderStep(const std::shared_ptr<RenderDevice>& renderDevice,
           const std::shared_ptr<RenderTarget>& renderTarget,
           const std::shared_ptr<RenderPipeline>& renderPipeline)
    : _renderDevice(renderDevice)
    , _renderTarget(renderTarget)
    , _renderPipeline(renderPipeline)
{
    init();
}

RenderStep::~RenderStep()
{
    destroy();
}

void RenderStep::draw(VkSemaphore waitVkSemaphore)
{
    destroyCommandBuffer();
    buildCommandBuffer();
    runCommandBuffer(waitVkSemaphore);
}

void RenderStep::init()
{
    auto graphicsQueue = _renderDevice->getGraphicsVkQueue();
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueue.second;
    poolInfo.flags = 0;
    
    if (vkCreateCommandPool(_renderDevice->getVkDevice(), &poolInfo, _renderDevice->allocator(), &_vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

void RenderStep::destroy()
{
    destroyCommandBuffer();
    if (_vkCommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(_renderDevice->getVkDevice(), _vkCommandPool, _renderDevice->allocator());
        _vkCommandPool = VK_NULL_HANDLE;
    }
}

void RenderStep::buildCommandBuffer()
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
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(_vkCommandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    _renderTarget->bind(_vkCommandBuffer);
    _renderPipeline->bind(_vkCommandBuffer);
    vkCmdDraw(_vkCommandBuffer, 3, 1, 0, 0);
    _renderTarget->unbind(_vkCommandBuffer);
    if (vkEndCommandBuffer(_vkCommandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void RenderStep::destroyCommandBuffer()
{
    if (_vkCommandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(_renderDevice->getVkDevice(), _vkCommandPool, 1, &_vkCommandBuffer);
        _vkCommandBuffer = VK_NULL_HANDLE;
    }
}

void RenderStep::runCommandBuffer(VkSemaphore waitVkSemaphore)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { waitVkSemaphore };
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_vkCommandBuffer;
    
    VkSemaphore signalSemaphores[] = { _renderTarget->getVkSemaphore() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    if (vkQueueSubmit(_renderDevice->getGraphicsVkQueue().first, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
}
