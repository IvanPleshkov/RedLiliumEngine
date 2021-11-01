#include "gpu_buffer.h"
#include <glm/glm.hpp>
#include <vector>

GpuBuffer::GpuBuffer(const std::shared_ptr<RenderDevice>& renderDevice, BufferType bufferType)
    : _renderDevice(renderDevice)
    , _bufferType(bufferType)
{
    init();
}

GpuBuffer::~GpuBuffer()
{
    destroy();
}

void GpuBuffer::init()
{
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;
        
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }
    };
    
    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
}

void GpuBuffer::destroy()
{
    
}
