#pragma once

#include "RenderCommon.h"
#include <Core/ApplicationSettings.h>
#include <Core/FileSystem.h>

namespace RED_LILIUM_NAMESPACE
{

class RenderDevice : public RedLiliumObject
{
public:
	RenderDevice(ptr<ApplicationSettings> applicationSettings);
	~RenderDevice() override;

	void Init(ptr<MaterialManager> materialManager);

	uptr<RenderContext> CreateRenderContext();

	void CheckErrors();
	ptr<ApplicationSettings> GetApplicationSettings();
	ptr<MaterialManager> GetMaterialManager();
	ptr<VertexDeclaration> GetVertexDeclaration(const std::vector<VertexAttribute>& attributes);

private:
	ptr<ApplicationSettings> m_applicationSettings;
	ptr<MaterialManager> m_materialManager;
	std::map<std::vector<VertexAttribute>, uptr<VertexDeclaration>> m_vertexDeclarations;
};

} // namespace RED_LILIUM_NAMESPACE
