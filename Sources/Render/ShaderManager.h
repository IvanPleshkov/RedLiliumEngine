#pragma once

#include "RenderCommon.h"
#include <Core/FileSystem.h>
#include "Shader.h"

namespace RED_LILIUM_NAMESPACE
{

class ShaderManager : public RedLiliumObject
{
public:
	ShaderManager(ptr<RenderDevice> renderDevice);
	~ShaderManager() override;

	sptr<Shader> GetShader(std::string_view filename, ShaderType type);
	sptr<ShaderProgram> GetShaderProgram(std::string_view filename);

private:
	sptr<Shader> NewShader(std::string_view filename, ShaderType type);
	sptr<ShaderProgram> NewShaderProgram(std::string_view filename);

	ptr<RenderDevice> m_renderDevice;

	std::map<std::string, sptr<Shader>, std::less<>> m_shaders;
	std::map<std::string, sptr<ShaderProgram>, std::less<>> m_shaderPrograms;
	std::map<std::string, sptr<Material>, std::less<>> m_materials;
};

} // namespace RED_LILIUM_NAMESPACE
