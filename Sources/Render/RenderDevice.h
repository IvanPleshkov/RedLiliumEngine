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

private:
	ptr<ApplicationSettings> m_applicationSettings;
	ptr<MaterialManager> m_materialManager;
};

} // namespace RED_LILIUM_NAMESPACE
