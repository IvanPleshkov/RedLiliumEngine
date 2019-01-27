#include "pch.h"
#include "RenderDevice.h"
#include "VertexDeclaration.h"
#include "RenderContext.h"

using namespace RED_LILIUM_NAMESPACE;

RenderDevice::RenderDevice(ptr<ApplicationSettings> applicationSettings)
	: RedLiliumObject()
	, m_applicationSettings(applicationSettings)
{
	InitStandardVertexDeclarations();
}

RenderDevice::~RenderDevice()
{
}

uptr<RenderContext> RenderDevice::CreateRenderContext()
{
	return std::move(umake<RenderContext>(this));
}

void RenderDevice::InitStandardVertexDeclarations()
{
	m_declP = umake<VertexDeclaration>(this, std::vector<VertexAttribute>({ VertexAttribute::Position }));
	m_declPN = umake<VertexDeclaration>(this, std::vector<VertexAttribute>({ VertexAttribute::Position, VertexAttribute::Normal }));
}

void RenderDevice::CheckErrors()
{
	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		RED_LILIUM_ASSERT(false && "gl error!");
	}
}
