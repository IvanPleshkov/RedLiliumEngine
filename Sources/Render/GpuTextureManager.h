#pragma once

#include "Render/RenderCommon.h"
#include <Core/FileSystem.h>
#include "GpuTexture.h"

namespace RED_LILIUM_NAMESPACE
{

class GpuTextureManager : public RedLiliumObject
{
public:
	GpuTextureManager(ptr<RenderDevice> renderDevice, ptr<FileSystem> fileSystem);
	~GpuTextureManager() override;

	const sptr<GpuTexture>& Get(const std::string& filename, const TextureSettings& settings);

private:
	ptr<RenderDevice> m_renderDevice;
	ptr<FileSystem> m_filesystem;
	std::map<std::string, sptr<GpuTexture>> m_textures;
};

} // namespace RED_LILIUM_NAMESPACE
