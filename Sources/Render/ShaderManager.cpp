#include "pch.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "VertexDeclaration.h"
#include "Material.h"
#include "RenderDevice.h"

using namespace RED_LILIUM_NAMESPACE;

ShaderManager::ShaderManager(ptr<RenderDevice> renderDevice)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_shaders()
	, m_shaderPrograms()
	, m_materials()
{
}

ShaderManager::~ShaderManager()
{
}

sptr<Shader> ShaderManager::GetShader(std::string_view filename, ShaderType type)
{
	auto i = m_shaders.find(filename);
	if (i != m_shaders.end())
	{
		if (i->second != nullptr)
		{
			return i->second;
		}
		else
		{
			m_shaders.erase(i);
		}
	}

	sptr<Shader> shader = NewShader(filename, type);
	m_shaders.insert({ std::string(filename), shader });
	return std::move(shader);
}

sptr<ShaderProgram> ShaderManager::GetShaderProgram(std::string_view filename)
{
	auto i = m_shaderPrograms.find(filename);
	if (i != m_shaderPrograms.end())
	{
		if (i->second != nullptr)
		{
			return i->second;
		}
		else
		{
			m_shaderPrograms.erase(i);
		}
	}

	sptr<ShaderProgram> shaderProgram = NewShaderProgram(filename);
	m_shaderPrograms.insert({ std::string(filename), shaderProgram });
	return std::move(shaderProgram);
}

sptr<Shader> ShaderManager::NewShader(std::string_view filename, ShaderType type)
{
	sptr<Shader> shader = smake<Shader>(m_renderDevice, filename);
	std::string shaderSource = m_renderDevice->GetFileSystem()->ReadShaderFile(filename);
	shader->CompileFromString(type, shaderSource);
	return std::move(shader);
}

sptr<ShaderProgram> ShaderManager::NewShaderProgram(std::string_view filename)
{
	json materialJson = m_renderDevice->GetFileSystem()->ReadJson(filename);
	RED_LILIUM_ASSERT(!materialJson.empty());

	const json& vertexShaderJson = materialJson["VertexShader"];
	RED_LILIUM_ASSERT(!vertexShaderJson.is_null());
	std::string vertexShaderFilename = vertexShaderJson["Filename"].get<std::string>();
	RED_LILIUM_ASSERT(!vertexShaderFilename.empty());
	sptr<Shader> vertexShader = GetShader(vertexShaderFilename, ShaderType::Vertex);

	const json& fragmentShaderJson = materialJson["FragmentShader"];
	RED_LILIUM_ASSERT(!fragmentShaderJson.is_null());
	std::string fragmentShaderFilename = fragmentShaderJson["Filename"].get<std::string>();
	RED_LILIUM_ASSERT(!fragmentShaderJson.empty());
	sptr<Shader> fragmentShader = GetShader(fragmentShaderFilename, ShaderType::Fragment);

	sptr<ShaderProgram> shaderProgram = smake<ShaderProgram>(m_renderDevice, filename);
	shaderProgram->Link(vertexShader, fragmentShader);
	return std::move(shaderProgram);
}
