#pragma once

#include "RenderCommon.h"
#include <Core/ApplicationSettings.h>
#include <Core/FileSystem.h>
#include "Uniform.h"

namespace RED_LILIUM_NAMESPACE
{

class RenderDevice : public RedLiliumObject
{
public:
	RenderDevice(ptr<ApplicationSettings> applicationSettings);
	~RenderDevice() override;

	void Init(ptr<MaterialManager> materialManager, ptr<GpuTextureManager> gpuTextureManager);

	uptr<RenderContext> CreateRenderContext();

	ptr<ApplicationSettings> GetApplicationSettings();
	ptr<MaterialManager> GetMaterialManager();
	ptr<GpuTextureManager> GetGpuTextureManager();

	ptr<VertexDeclaration> GetVertexDeclaration(const std::vector<VertexInput>& attributes);
	std::optional<std::pair<ptr<Uniform>, ptr<UniformBlock>>> GetGlobalUniform(std::string_view name);
	ptr<UniformBlock> GetUniformBlock(std::string_view name);
	ptr<UniformBlock> GetUniformBlock(ptr<ShaderProgram> program, std::string_view name);

private:
	ptr<ApplicationSettings> m_applicationSettings;
	ptr<MaterialManager> m_materialManager;
	ptr<GpuTextureManager> m_gpuTextureManager;
	std::map<std::string, uptr<VertexDeclaration>, std::less<>> m_vertexDeclarations;
	std::map<std::string, uptr<UniformBlock>, std::less<>> m_uniformBlocks;
	std::map<std::string, std::pair<ptr<Uniform>, ptr<UniformBlock>>, std::less<>> m_globalUniforms;
};

} // namespace RED_LILIUM_NAMESPACE
