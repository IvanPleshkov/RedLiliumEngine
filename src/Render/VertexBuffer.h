#pragma once

#include "RenderTarget.h"

namespace RED_LILIUM_NAMESPACE
{

class VertexBuffer
{
public:


private:
	bgfx::VertexBufferHandle m_handle;
	sptr<bgfx::VertexDecl> m_vertexDeclaration;
};

} // namespace RED_LILIUM_NAMESPACE
