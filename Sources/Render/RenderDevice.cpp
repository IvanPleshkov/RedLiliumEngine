#include "pch.h"
#include "RenderDevice.h"
#include "VertexDeclaration.h"
#include "RenderContext.h"

using namespace RED_LILIUM_NAMESPACE;

namespace
{
static void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string msgSource;
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		msgSource = "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		msgSource = "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		msgSource = "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		msgSource = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		msgSource = "OTHER";
		break;
	}

	std::string msgType;
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		msgType = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		msgType = "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		msgType = "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		msgType = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		msgType = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		msgType = "OTHER";
		break;
	}

	std::string msg = 
		std::string("glDebugMessage:\n") + 
		std::string(message) + 
		std::string("\nType = ") + msgType + 
		std::string(" Source = ") + msgSource;

	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		RED_LILIUM_LOG_WARNING(msg);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		RED_LILIUM_LOG_ERROR(msg);
		RED_LILIUM_ASSERT(false && "OpenGL message");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		RED_LILIUM_LOG_CRITICAL(msg);
		RED_LILIUM_ASSERT(false && "OpenGL message");
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		RED_LILIUM_LOG_DEBUG(msg);
		break;
	default:
		RED_LILIUM_LOG_CRITICAL(msg);
		RED_LILIUM_ASSERT(false && "OpenGL message");
		break;
	}
}
}

RenderDevice::RenderDevice(ptr<ApplicationSettings> applicationSettings)
	: RedLiliumObject()
	, m_applicationSettings(applicationSettings)
{
#ifdef RED_LILIUM_RENDER_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glDebugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
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

ptr<ApplicationSettings> RenderDevice::GetApplicationSettings()
{
	return m_applicationSettings;
}

ptr<MaterialManager> RenderDevice::GetMaterialManager()
{
	return m_materialManager;
}

ptr<VertexDeclaration> RenderDevice::GetVertexDeclaration(const std::vector<VertexInput>& attributes)
{
	std::string hashString;
	hashString.reserve(attributes.size() * 2);
	for (auto& a : attributes)
	{
		hashString.push_back(static_cast<char>(a.layout));
		hashString.push_back(static_cast<char>(a.vertexAttribute));
	}

	auto i = m_vertexDeclarations.find(hashString);
	if (i == m_vertexDeclarations.end())
	{
		auto j = m_vertexDeclarations.insert({ std::move(hashString), umake<VertexDeclaration>(this, attributes) });
		RED_LILIUM_ASSERT(j.second);
		i = j.first;
	}
	return i->second.get();
}
