#pragma once

#include <memory>

class RenderDevice;
class GpuBuffer;
class Mesh;

class GpuMesh
{
public:
    GpuMesh(std::shared_ptr<RenderDevice> renderDevice);

    ~GpuMesh();
    
    void update(const Mesh& mesh);
    
    const std::shared_ptr<GpuBuffer> getVertexBuffer() const;
    
    const std::shared_ptr<GpuBuffer> getIndexBuffer() const;
    
    size_t getIndicesCount() const;
    
private:
    std::shared_ptr<RenderDevice> _renderDevice;
    std::shared_ptr<GpuBuffer> _vertexBuffer;
    std::shared_ptr<GpuBuffer> _indexBuffer;
    size_t _indicesCount;
};
