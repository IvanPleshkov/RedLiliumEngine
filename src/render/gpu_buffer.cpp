#include "gpu_buffer.h"
#include "render_device.h"
#include <glm/glm.hpp>
#include <vector>

GpuBuffer::GpuBuffer(const std::shared_ptr<RenderDevice>& renderDevice, BufferType bufferType)
    : _renderDevice(renderDevice)
    , _bufferType(bufferType)
{
}

GpuBuffer::~GpuBuffer()
{
    destroy();
}

void GpuBuffer::update(const char* data, size_t size)
{
    destroy();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    
    switch (_bufferType)
    {
        case Index:
            bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        case Vertex:
            bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        default:
            throw std::runtime_error("not supported buffer type");
    }
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(_renderDevice->getVkDevice(), &bufferInfo, _renderDevice->allocator(), &_vkVertexBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_renderDevice->getVkDevice(), _vkVertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _renderDevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(_renderDevice->getVkDevice(), &allocInfo, _renderDevice->allocator(), &_vkVertexBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    vkBindBufferMemory(_renderDevice->getVkDevice(), _vkVertexBuffer, _vkVertexBufferMemory, 0);
    
    void* gpuData;
    vkMapMemory(_renderDevice->getVkDevice(), _vkVertexBufferMemory, 0, bufferInfo.size, 0, &gpuData);
    memcpy(gpuData, data, size);
    vkUnmapMemory(_renderDevice->getVkDevice(), _vkVertexBufferMemory);
}

VkBuffer GpuBuffer::getVkBuffer() const
{
    return _vkVertexBuffer;
}

void GpuBuffer::destroy()
{
    vkDestroyBuffer(_renderDevice->getVkDevice(), _vkVertexBuffer, _renderDevice->allocator());
    vkFreeMemory(_renderDevice->getVkDevice(), _vkVertexBufferMemory, nullptr);
}
