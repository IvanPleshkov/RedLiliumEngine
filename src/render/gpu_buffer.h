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
        Index
    };

    GpuBuffer(const std::shared_ptr<RenderDevice>& renderDevice, BufferType bufferType);

    ~GpuBuffer();

    void update(const char* data, size_t size);
    
    VkBuffer getVkBuffer() const;

private:
    void init();

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    BufferType _bufferType;
    VkBuffer _vkVertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory _vkVertexBufferMemory = VK_NULL_HANDLE;
};
