#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"
#include <Core/Flags.h>

namespace RED_LILIUM_NAMESPACE
{

class VertexDeclaration : public RedLiliumObject
{
public:
	VertexDeclaration(ptr<RenderDevice> renderDevice, const std::vector<VertexInput>& verts);
	~VertexDeclaration() override;

	const std::vector<VertexInput>& GetVertexElements() const;

private:
	ptr<RenderDevice> m_renderDevice;
	std::vector<VertexInput> m_vertexElements;
};

class VertexArrayObject : public GpuResource
{
public:
	VertexArrayObject(ptr<RenderDevice> renderDevice, ptr<VertexDeclaration> vertexDeclaration);
	~VertexArrayObject() override;
};

} // namespace RED_LILIUM_NAMESPACE
