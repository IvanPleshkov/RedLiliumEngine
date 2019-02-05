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
	
	sptr<Material> Get(std::string_view filename, bool createCopy = false);

private:
	sptr<Shader> GetShader(std::string_view filename, ShaderType type);
	sptr<ShaderProgram> GetShaderProgram(std::string_view filename, const json& materialJson);

	sptr<Material> NewMaterial(std::string_view filename);
	sptr<Shader> NewShader(std::string_view filename, ShaderType type);
	sptr<ShaderProgram> NewShaderProgram(std::string_view filename, const json& materialJson);

	ptr<RenderDevice> m_renderDevice;
	ptr<FileSystem> m_fileSystem;

	std::map<std::string, sptr<Shader>, std::less<>> m_shaders;
	std::map<std::string, sptr<ShaderProgram>, std::less<>> m_shaderPrograms;
	std::map<std::string, sptr<Material>, std::less<>> m_materials;
};

} // namespace RED_LILIUM_NAMESPACE
