#pragma once

#include "RenderCommon.h"
#include <Core/FileSystem.h>
#include "Shader.h"

namespace RED_LILIUM_NAMESPACE
{

class MaterialManager : public RedLiliumObject
{
public:
	MaterialManager(ptr<RenderDevice> renderDevice, ptr<FileSystem> fileSystem);
	~MaterialManager() override;
	
	sptr<Material> Get(const std::string& filename, bool createCopy = false);

private:
	sptr<Shader> GetShader(const std::string& filename, ShaderType type);
	sptr<ShaderProgram> GetShaderProgram(const std::string& filename, const json& materialJson);
	ptr<VertexDeclaration> GetVertexDeclaration(const json& materialJson);

	sptr<Material> NewMaterial(const std::string& filename);
	sptr<Shader> NewShader(const std::string& filename, ShaderType type);
	sptr<ShaderProgram> NewShaderProgram(const std::string& filename, const json& materialJson);

	ptr<RenderDevice> m_renderDevice;
	ptr<FileSystem> m_fileSystem;

	std::map<std::string, uptr<VertexDeclaration>> m_vertexDeclarations;
	std::map<std::string, sptr<Shader>> m_shaders;
	std::map<std::string, sptr<ShaderProgram>> m_shaderPrograms;
	std::map<std::string, sptr<Material>> m_materials;
};

} // namespace RED_LILIUM_NAMESPACE
