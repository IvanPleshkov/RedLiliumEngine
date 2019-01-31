#include "pch.h"
#include "RenderDevice.h"
#include "VertexDeclaration.h"
#include "RenderContext.h"

using namespace RED_LILIUM_NAMESPACE;

RenderDevice::RenderDevice(ptr<ApplicationSettings> applicationSettings)
	: RedLiliumObject()
	, m_applicationSettings(applicationSettings)
{
}

RenderDevice::~RenderDevice()
{
}

void RenderDevice::Init(ptr<MaterialManager> materialManager)
{
	m_materialManager = materialManager;
}

uptr<RenderContext> RenderDevice::CreateRenderContext()
{
	return std::move(umake<RenderContext>(this));
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

ptr<ApplicationSettings> RenderDevice::GetApplicationSettings()
{
	return m_applicationSettings;
}

ptr<MaterialManager> RenderDevice::GetMaterialManager()
{
	return m_materialManager;
}

ptr<VertexDeclaration> RenderDevice::GetVertexDeclaration(const std::vector<VertexAttribute>& attributes)
{
	auto i = m_vertexDeclarations.find(attributes);
	if (i == m_vertexDeclarations.end())
	{
		auto j = m_vertexDeclarations.insert({ attributes, umake<VertexDeclaration>(this, attributes) });
		RED_LILIUM_ASSERT(j.second);
		i = j.first;
	}
	return i->second.get();
}
