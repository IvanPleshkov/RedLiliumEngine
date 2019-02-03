#include "pch.h"
#include "RenderDevice.h"
#include "VertexDeclaration.h"
#include "RenderContext.h"
#include "Uniform.h"

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

	glEnable(GL_MULTISAMPLE);
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

ptr<UniformBlock> RenderDevice::GetUniformBlock(const std::string& name)
{
	auto i = m_uniformBlocks.find(name);
	if (i == m_uniformBlocks.end())
	{
		std::string s = "Uniform block '" + name + "' not found";
		RED_LILIUM_LOG_WARNING(s);
		return nullptr;
	}

	return i->second.get();
}

ptr<UniformBlock> RenderDevice::GetUniformBlock(ptr<ShaderProgram> program, const std::string& name)
{
	auto i = m_uniformBlocks.find(name);
	if (i == m_uniformBlocks.end())
	{
		uptr<UniformBlock> uniformBlock = umake<UniformBlock>(program, name);
		ptr<UniformBlock> result = uniformBlock.get();
		m_uniformBlocks.insert({ name, std::move(uniformBlock) });

		auto& uniforms = result->GetUniforms();
		for (auto& uniform : uniforms)
		{
			if (auto i = m_globalUniforms.find(uniform.GetName()); i != m_globalUniforms.end())
			{
				std::string s = "Gloabl Uniform " + uniform.GetName() + " is already present in block " + i->second.second->GetName();
				RED_LILIUM_LOG_ERROR(s);
				RED_LILIUM_ASSERT(false && "Gloabl Uniform is already present in other uniform block");
			}
			m_globalUniforms.insert({ uniform.GetName(), { uniform, result } });
		}

		return result;
	}

	i->second->Check(program);
	return i->second.get();
}

std::optional<std::pair<Uniform, ptr<UniformBlock>>> RenderDevice::GetGlobalUniform(const std::string& name)
{
	auto i = m_globalUniforms.find(name);
	if (i == m_globalUniforms.end())
	{
		return std::nullopt;
	}

	return i->second;
}
