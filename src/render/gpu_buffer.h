#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class RenderDevice;
class RenderStep;

class GpuBuffer : public std::enable_shared_from_this<GpuBuffer>
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

    void update(const std::shared_ptr<RenderStep>& renderStep, const char* data, size_t size);
    
    void download(char* data, size_t size);

    VkBuffer getVkBuffer() const;
    
    size_t getSize() const;

private:
    void init();

    void destroy();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    std::shared_ptr<RenderDevice> _renderDevice;
    size_t _size = 0;
    BufferType _bufferType;
    bool _useStagingBuffers = true;
    VkBuffer _vkBuffer = VK_NULL_HANDLE;
    VkDeviceMemory _vkBufferMemory = VK_NULL_HANDLE;
    VkBuffer _vkStagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory _vkStagingBufferMemory = VK_NULL_HANDLE;
};
