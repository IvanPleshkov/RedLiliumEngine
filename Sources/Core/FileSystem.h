#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class ApplicationSettings;
class TextureBase;

class FileSystem : public RedLiliumObject
{
public:
	FileSystem(ptr<ApplicationSettings> applicationSettings);
	~FileSystem() override = default;

	std::string ReadFile(const std::string& filename);
	json ReadJson(const std::string& filename);
	cv::Mat ReadTexture(const std::string& filename, int cvFlags = cv::IMREAD_UNCHANGED);

private:
	ptr<ApplicationSettings> m_applicationSettings;
	std::string m_workingPath;
};

} // namespace RED_LILIUM_NAMESPACE
