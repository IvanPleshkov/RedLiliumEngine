#include "pch.h"
#include "MaterialManager.h"
#include "Shader.h"
#include "VertexDeclaration.h"
#include "Material.h"

using namespace RED_LILIUM_NAMESPACE;

MaterialManager::MaterialManager(ptr<RenderDevice> renderDevice, ptr<FileSystem> fileSystem)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_fileSystem(fileSystem)
	, m_shaders()
	, m_shaderPrograms()
	, m_materials()
{
}

MaterialManager::~MaterialManager()
{
}

sptr<Material> MaterialManager::Get(std::string_view filename, bool createCopy)
{
	if (createCopy)
	{
		return NewMaterial(filename);
	}

	auto i = m_materials.find(filename);
	if (i != m_materials.end())
	{
		if (i->second != nullptr)
		{
			return i->second;
		}
		else
		{
			m_materials.erase(i);
		}
	}

	sptr<Material> material = NewMaterial(filename);
	m_materials.insert({ std::string(filename), material });
	return std::move(material);
}

sptr<Shader> MaterialManager::GetShader(std::string_view filename, ShaderType type)
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

sptr<ShaderProgram> MaterialManager::GetShaderProgram(std::string_view filename, const json& materialJson)
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

	sptr<ShaderProgram> shaderProgram = NewShaderProgram(filename, materialJson);
	m_shaderPrograms.insert({ std::string(filename), shaderProgram });
	return std::move(shaderProgram);
}

sptr<Material> MaterialManager::NewMaterial(std::string_view filename)
{
	json materialJson = m_fileSystem->ReadJson(filename);
	sptr<ShaderProgram> shaderProgram = GetShaderProgram(filename, materialJson);

	sptr<Material> material = smake<Material>(m_renderDevice, filename);
	material->SetShaderProgram(shaderProgram);

	return std::move(material);
}

sptr<Shader> MaterialManager::NewShader(std::string_view filename, ShaderType type)
{
	sptr<Shader> shader = smake<Shader>(m_renderDevice);
	std::string shaderSource = m_fileSystem->ReadShaderFile(filename);
	shader->CompileFromString(type, shaderSource);
	return std::move(shader);
}

sptr<ShaderProgram> MaterialManager::NewShaderProgram(std::string_view filename, const json& materialJson)
{
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

	sptr<ShaderProgram> shaderProgram = smake<ShaderProgram>(m_renderDevice);
	shaderProgram->Link(vertexShader, fragmentShader);
	return std::move(shaderProgram);
}
