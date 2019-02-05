#include "pch.h"
#include "GpuTextureManager.h"

using namespace RED_LILIUM_NAMESPACE;

GpuTextureManager::GpuTextureManager(ptr<RenderDevice> renderDevice, ptr<FileSystem> fileSystem)
	: RedLiliumObject()
	, m_renderDevice(renderDevice)
	, m_filesystem(fileSystem)
{
}

GpuTextureManager::~GpuTextureManager()
{
}

const sptr<GpuTexture>& GpuTextureManager::Get(std::string_view filename, const TextureSettings& settings)
{
	auto i = m_textures.find(filename);
	if (i != m_textures.end())
	{
		return i->second;
	}

	sptr<GpuTexture> texture = smake<GpuTexture>(m_renderDevice, settings);
	cv::Mat mat = m_filesystem->ReadTexture(filename);
	texture->SendData(mat);
	m_textures.insert({ std::string(filename), texture });
	return texture;
}
