#include "gpu_mesh.h"
#include "render_device.h"
#include "gpu_buffer.h"
#include "../mesh.h"

GpuMesh::GpuMesh(std::shared_ptr<RenderDevice> renderDevice)
    : _renderDevice(renderDevice)
{
    
}

GpuMesh::~GpuMesh() {}

void GpuMesh::update(const Mesh& mesh)
{
    _vertexBuffer = std::make_shared<GpuBuffer>(_renderDevice, GpuBuffer::Vertex);
    _vertexBuffer->update(reinterpret_cast<const char*>(mesh._vertices.data()), sizeof(Vertex) * mesh._vertices.size());
    if (!mesh._indices.empty())
    {
        _indexBuffer = std::make_shared<GpuBuffer>(_renderDevice, GpuBuffer::Index);
    }
    else
    {
        _indexBuffer = nullptr;
    }
}

const std::shared_ptr<GpuBuffer> GpuMesh::getVertexBuffer() const
{
    return _vertexBuffer;
}

const std::shared_ptr<GpuBuffer> GpuMesh::getIndexBuffer() const
{
    return _indexBuffer;
}
