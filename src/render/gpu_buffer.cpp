#include "gpu_buffer.h"
#include "render_device.h"
#include "render_context.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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
        init();
    }
    
    if (_useStagingBuffers)
    {
        void* gpuData;
        vkMapMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory, 0, vkSize, 0, &gpuData);
        std::memcpy(gpuData, data, size);
        vkUnmapMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory);

        auto renderContext = std::make_shared<RenderContext>(_renderDevice, _renderDevice->getGraphicsVkQueue().first, _renderDevice->getGraphicsVkQueue().second);
        renderContext->copyBuffer(_vkStagingBuffer, _vkBuffer, vkSize);
        renderContext->run(VK_NULL_HANDLE, VK_NULL_HANDLE);
    }
    else
    {
        void* gpuData;
        vkMapMemory(_renderDevice->getVkDevice(), _vkBufferMemory, 0, vkSize, 0, &gpuData);
        std::memcpy(gpuData, data, size);
        vkUnmapMemory(_renderDevice->getVkDevice(), _vkBufferMemory);
    }
}

void GpuBuffer::update(const std::shared_ptr<RenderContext>& renderContext, const char* data, size_t size)
{
    if (!_useStagingBuffers)
    {
        throw std::runtime_error("cannot upload gpu buffer because buffer was initialized without staging flag");
    }

    const VkDeviceSize vkSize = static_cast<VkDeviceSize>(size);
    if (_size != size)
    {
        destroy();
        _size = size;
        init();
    }

    void* gpuData;
    vkMapMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory, 0, vkSize, 0, &gpuData);
    std::memcpy(gpuData, data, size);
    vkUnmapMemory(_renderDevice->getVkDevice(), _vkStagingBufferMemory);

    renderContext->copyBuffer(_vkStagingBuffer, _vkBuffer, vkSize);
}

void GpuBuffer::download(char* data, size_t size)
{
    if (_useStagingBuffers)
    {
        throw std::runtime_error("cannot download gpu buffer because buffer was initialized without staging flag");
    }
    
    const VkDeviceSize vkSize = static_cast<VkDeviceSize>(size);
    void* gpuData;
    vkMapMemory(_renderDevice->getVkDevice(), _vkBufferMemory, 0, vkSize, 0, &gpuData);
    std::memcpy(data, gpuData, size);
    vkUnmapMemory(_renderDevice->getVkDevice(), _vkBufferMemory);
}

VkBuffer GpuBuffer::getVkBuffer() const
{
    return _vkBuffer;
}

size_t GpuBuffer::getSize() const
{
    return _size;
}

void GpuBuffer::init()
{
    VkBufferUsageFlags usageFlags{};
    switch (_bufferType)
    {
        case Index:
            usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
        case Vertex:
            usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
        case Uniform:
            usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
        default:
            throw std::runtime_error("not supported buffer type");
    }

    if (_useStagingBuffers)
    {
        createBuffer(_size, usageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vkStagingBuffer, _vkStagingBufferMemory);
        createBuffer(_size, usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vkBuffer, _vkBufferMemory);
    }
    else
    {
        createBuffer(_size, usageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vkBuffer, _vkBufferMemory);
    }
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
