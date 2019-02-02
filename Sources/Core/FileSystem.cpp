#include "pch.h"
#include "FileSystem.h"
#include "ApplicationSettings.h"
#include "Texture.h"

using namespace RED_LILIUM_NAMESPACE;

FileSystem::FileSystem(ptr<ApplicationSettings> applicationSettings)
	: RedLiliumObject()
	, m_applicationSettings(applicationSettings)
	, m_workingPath(applicationSettings->GetResourcesPath())
{
}

std::string FileSystem::ReadFile(const std::string& filename)
{
	const std::string fullFilename = m_workingPath + "\\" + filename;
	std::ifstream fileStream(fullFilename);
	std::string fileData(
		(std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());
	return std::move(fileData);
}

json FileSystem::ReadJson(const std::string& filename)
{
	std::string fileData = ReadFile(filename);
	json j = json::parse(fileData);
	return std::move(j);
}

uptr<TextureBase> FileSystem::ReadTexture(const std::string& filename)
{
	const std::string fullFilename = m_workingPath + "\\" + filename;
	uptr<TextureBase> texture;

	i32 width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image = stbi_load(
		fullFilename.c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb_alpha);

	stbi_image_free(image);

	return std::move(texture);
}
