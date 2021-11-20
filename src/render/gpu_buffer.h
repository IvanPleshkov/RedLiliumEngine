#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class RenderDevice;

class GpuBuffer
{
public:
    enum BufferType
    {
        Vertex,
        Index,
        Uniform
    };

    GpuBuffer(const std::shared_ptr<RenderDevice>& renderDevice, BufferType bufferType, bool useStagingBuffers = true);

    ~GpuBuffer();

    void update(const char* data, size_t size);
    
    VkBuffer getVkBuffer() const;

private:
    void init();

    void destroy();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    std::shared_ptr<RenderDevice> _renderDevice;
    size_t _size = 0;
    BufferType _bufferType;
    bool _useStagingBuffers = true;
    VkBuffer _vkBuffer = VK_NULL_HANDLE;
    VkDeviceMemory _vkBufferMemory = VK_NULL_HANDLE;
    VkBuffer _vkStagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory _vkStagingBufferMemory = VK_NULL_HANDLE;
};
