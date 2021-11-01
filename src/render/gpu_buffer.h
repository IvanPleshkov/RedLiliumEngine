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

    void updateImmediate();

    void update();

private:
    void init();

    void destroy();

    std::shared_ptr<RenderDevice> _renderDevice;
    BufferType _bufferType;
};
