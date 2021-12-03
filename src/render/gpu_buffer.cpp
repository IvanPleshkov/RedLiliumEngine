#include "gpu_buffer.h"
#include "render_device.h"
#include <glm/glm.hpp>
#include <vector>

GpuBuffer::GpuBuffer(const std::shared_ptr<RenderDevice>& renderDevice, BufferType bufferType, bool useStagingBuffers)
    : _renderDevice(renderDevice)
    , _bufferType(bufferType)
    , _useStagingBuffers(useStagingBuffers)
{
}

GpuBuffer::~GpuBuffer()
{
    destroy();
}

void GpuBuffer::update(const char* data, size_t size)
{
    const VkDeviceSize vkSize = static_cast<VkDeviceSize>(size);
    if (_size != size)
    {
        destroy();
        _size = size;

        VkBufferUsageFlags usageFlags{};
        switch (_bufferType)
        {
            case Index:
            usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
            case Vertex:
                usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                break;
            case Uniform:
                usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                break;
            default:
                throw std::runtime_error("not supported buffer type");
        }

        if (_useStagingBuffers)
        {
            createBuffer(vkSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vkStagingBuffer, _vkStagingBufferMemory);
            createBuffer(vkSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vkBuffer, _vkBufferMemory);
        }
        else
        {
            createBuffer(vkSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vkBuffer, _vkBufferMemory);
        }
    }
    
    if (_useStagingBuffers)
    {
        void* gpuData;
        vkMapMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory, 0, vkSize, 0, &gpuData);
        memcpy(gpuData, data, size);
        vkUnmapMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory);
        copyBuffer(_vkStagingBuffer, _vkBuffer, vkSize);
    }
    else
    {
        void* gpuData;
        vkMapMemory(_renderDevice->getVkDevice(), _vkBufferMemory, 0, vkSize, 0, &gpuData);
        memcpy(gpuData, data, size);
        vkUnmapMemory(_renderDevice->getVkDevice(), _vkBufferMemory);
    }
}

VkBuffer GpuBuffer::getVkBuffer() const
{
    return _vkBuffer;
}

size_t GpuBuffer::getSize() const
{
    return _size;
}

void GpuBuffer::destroy()
{
    _size = 0;
    if (_vkStagingBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(_renderDevice->getVkDevice(), _vkStagingBuffer, _renderDevice->allocator());
        _vkStagingBuffer = VK_NULL_HANDLE;
    }
    if (_vkStagingBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory, _renderDevice->allocator());
        _vkStagingBufferMemory = VK_NULL_HANDLE;
    }
    if (_vkBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(_renderDevice->getVkDevice(), _vkBuffer, _renderDevice->allocator());
        _vkBuffer = VK_NULL_HANDLE;
    }
    if (_vkBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(_renderDevice->getVkDevice(), _vkBufferMemory, _renderDevice->allocator());
        _vkBufferMemory = VK_NULL_HANDLE;
    }
}

void GpuBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags vkProperties, VkBuffer& vkBuffer, VkDeviceMemory& vkBufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(_renderDevice->getVkDevice(), &bufferInfo, _renderDevice->allocator(), &vkBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create gpu buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_renderDevice->getVkDevice(), vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _renderDevice->findMemoryType(memRequirements.memoryTypeBits, vkProperties);

    if (vkAllocateMemory(_renderDevice->getVkDevice(), &allocInfo, _renderDevice->allocator(), &vkBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate gpu buffer memory!");
    }
    vkBindBufferMemory(_renderDevice->getVkDevice(), vkBuffer, vkBufferMemory, 0);
}

void GpuBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
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

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(vkCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    
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
