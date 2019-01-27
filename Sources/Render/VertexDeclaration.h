#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"
#include <Core/Flags.h>

namespace RED_LILIUM_NAMESPACE
{

class VertexDeclaration : public RedLiliumObject
{
public:
	VertexDeclaration(ptr<RenderDevice> renderDevice, const std::vector<VertexAttribute>& vertexElements);
	~VertexDeclaration() override;

	const std::vector<VertexAttribute>& GetVertexElements() const;

private:
	ptr<RenderDevice> m_renderDevice;
	std::vector<VertexAttribute> m_vertexElements;
};


class VertexArrayObject : public GpuResource
{
public:
	VertexArrayObject(ptr<RenderDevice> renderDevice, ptr<VertexDeclaration> vertexDeclaration);
	~VertexArrayObject() override;

private:
	VertexAttributeFlags m_vertexAttributeFlags;
};

} // namespace RED_LILIUM_NAMESPACE
